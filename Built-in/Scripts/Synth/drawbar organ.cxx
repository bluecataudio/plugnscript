/** \file
*   Drawbar organ.
*   Simple drawbar organ instrument, demonstrating simple additive synthesis with sine waveforms.
*/

#include "../library/Midi.hxx"
#include "../library/Constants.hxx"

// dsp script interface--------------------------

string name="Mini Drawbar Organ";
string description="Simple drawbar organ";

array<string> inputParametersNames={"Smooth", "Bar 1", "Bar 2", "Bar 3", "Bar 4" , "Bar 5", "Gain"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersDefault={0.01,0,1,0,0,0,0.5};

enum EParams
{
    kSmoothParam=0,
    kBar1Param,
    kBar2Param,
    kBar3Param,
    kBar4Param,
    kBar5Param,
    kGainParam
};

void processBlock(BlockData& data)
{
    // smooth gain update: use begin and end values
    // since the actual gain is exponential, we can use the ratio between begin and end values
    // as an incremental multiplier for the actual gain
    double gainDiff=data.endParamValues[kGainParam]-data.beginParamValues[kGainParam];
    double gainRatio=1;
    if(gainDiff!=0)
        gainRatio=pow(10,gainDiff/double(data.samplesToProcess)*2);

    uint nextEventIndex=0;
    for(uint i=0;i<data.samplesToProcess;i++)
    {
        // manage MIDI events
        while(nextEventIndex!=data.inputMidiEvents.length && data.inputMidiEvents[nextEventIndex].timeStamp<=double(i))
        {
            handleMidiEvent(data.inputMidiEvents[nextEventIndex]);
            nextEventIndex++;
        }

        // compute sample value
        double sampleValue=0;
        for(uint v=0;v<activeVoicesCount;v++)
        {
            sampleValue+=voices[v].ProcessSample();
        }
        sampleValue*=gain;

        // copy value to all outputs
        for(uint ch=0;ch<audioOutputsCount;ch++)
        {
            data.samples[ch][i]=sampleValue;
        }

        // update the gain
        gain*=gainRatio;
    }

    // to avoid overflow, reduce phase for all active voices
    for(uint i=0;i<activeVoicesCount;i++)
    {
        voices[i].ReducePhase();
    }
}

void updateInputParametersForBlock(const TransportInfo@ info)
{
    amplitudeCoeff=pow(10,1.0/(50+.5*sampleRate*inputParameters[0]))-1;
    bar1Level=inputParameters[kBar1Param];
    bar2Level=inputParameters[kBar2Param];
    bar3Level=inputParameters[kBar3Param];
    bar4Level=inputParameters[kBar4Param];
    bar5Level=inputParameters[kBar5Param];
    gain=pow(10,-1+inputParameters[kGainParam]*2);
}

int getTailSize()
{
    return -1;
}

// internal processing properties and functions---------------------
double currentPitchOffset=0;
const double period=4*PI; //2*2PI because least common denomiator is half frequency (sub octave)

double amplitudeCoeff=0;
double gain=0;
double bar1Level=0;
double bar2Level=0;
double bar3Level=0;
double bar4Level=0;
double bar5Level=0;
bool pedalIsDown=false;
uint activeVoicesCount=0;
array<SynthVoice> voices(24);
SynthVoice tempVoice;

void handleMidiEvent(const MidiEvent& evt)
{
    switch(MidiEventUtils::getType(evt))
    {
    case kMidiNoteOn:
        {
            // find voice for the note on event (in case it is already here)
            // or use the first empty slot (note=-1)
            tempVoice.currentNote=MidiEventUtils::getNote(evt);
            int index=voices.find(tempVoice);
            if(index<0)
            {
                if(activeVoicesCount<voices.length)
                {
                    index=activeVoicesCount;
                    activeVoicesCount++;
                }
            }
            if(index>=0)
            {
                voices[index].NoteOn(evt);
            }
            break;
        }
    case kMidiNoteOff:
        {
            // find voice for the note off event
            tempVoice.currentNote=MidiEventUtils::getNote(evt);
            int index=voices.find(tempVoice);
            if(index>=0)
            {
                voices[index].NoteOff();
            }
            break;
        }
    case kMidiPitchWheel:
        {
            // update pitch for all voices
            currentPitchOffset=2*double(MidiEventUtils::getPitchWheelValue(evt))/8192.0;
            for(uint i=0;i<activeVoicesCount;i++)
            {
                if(voices[i].omega>0)
                {
                    voices[i].ForcePitch();
                }
            }
            break;
        }
    case kMidiControlChange:
        {
            // sustain pedal changed event
            if(MidiEventUtils::getCCNumber(evt)==64)
            {
                // if pedal is down
                bool down=(MidiEventUtils::getCCValue(evt)>=64);
                if(down!=pedalIsDown)
                {
                    pedalIsDown=down;
                    if(pedalIsDown==false)
                    {
                        for(uint i=0;i<activeVoicesCount;i++)
                        {
                            voices[i].PedalReleased();
                        }
                    }
                }
            }
        }
    }
}

/** Class that manages a single voice for the synth.
*
*/
class SynthVoice
{
    double amplitude=0;
    double omega=0;

    double currentAmplitude=0;
    double currentPhase=0;

    int    currentNote=-1;
    bool   waitingForPedalRelease;

    // handle note on operation
    void NoteOn(const MidiEvent& evt)
    {
        amplitude=double(MidiEventUtils::getNoteVelocity(evt))/127.0;
        currentNote=MidiEventUtils::getNote(evt);
        omega=2*PI*pow(2,((double(currentNote-69.0)+currentPitchOffset)/12.0))*440.0/sampleRate;
    }

    void NoteOff()
    {
        if(!pedalIsDown)
            // set amplitude to zero. Voice will be freed only when amplitude gets close to zero
                amplitude=0;
        else
            // remember that note off has been called - will actually release the note later, when the pedal is released
            waitingForPedalRelease=true;
    }

    void ForcePitch()
    {
        omega=2*PI*pow(2,((double(currentNote-69.0)+currentPitchOffset)/12.0))*440.0/sampleRate;
    }

    void PedalReleased()
    {
        if(waitingForPedalRelease)
        {
            NoteOff();
            waitingForPedalRelease=false;
        }
    }

    // returns true if voice ended
    double ProcessSample()
    {
        double sampleValue=0;

        // update amplitude
        currentAmplitude+=amplitudeCoeff*(amplitude-currentAmplitude);
        if(amplitude==0 && currentAmplitude<.0001)
        {
            // value below threshold => the voice ended

            // if not the last voice in the list
            if(voices[activeVoicesCount-1]!=this)
            {
                // swap with last voice in the list
                this=voices[activeVoicesCount-1];
                voices[activeVoicesCount-1].CancelNote();

                // decrease voices count
                activeVoicesCount--;

                // process swaped voice instead of this one
                return ProcessSample();
            }
            else
            {
                // cancel current note
                CancelNote();

                // decrease voices count
                activeVoicesCount--;
                return 0;
            }
        }

        // compute sample value
        sampleValue=currentAmplitude*(bar1Level*sin(currentPhase*.5)+bar2Level*sin(currentPhase)+bar3Level*sin(currentPhase*1.5)+bar4Level*sin(currentPhase*2)+bar5Level*sin(currentPhase*3));

        // update phase
        currentPhase+=omega;
        return sampleValue;
    }

    void CancelNote()
    {
        currentNote=-1;
        currentPhase=0;
        currentAmplitude=0;
        omega=0;
        waitingForPedalRelease=false;
    }

    void ReducePhase()
    {
        // warning: this can cause an almost infinite loop is currentPhase not properly updated
        while(currentPhase>period)
            currentPhase-=period;
    }

    // == operator for find operations
    bool opEquals(const SynthVoice& voice)const
    {
        return currentNote==voice.currentNote;
    }
};
