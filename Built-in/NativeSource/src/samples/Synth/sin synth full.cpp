// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioOutputsCount=0;

// extra system headers
#include <math.h>

/** \file
*   Polyphomic sine wave synth with amplitude and pitch envelopes.
*/

#include "../library/Midi.h"
#include "../library/Constants.h"

// dsp script interface--------------------------

DSP_EXPORT string name="Sin Synth Full";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Simple sine wave synth (polyphonic)";

DSP_EXPORT array<string> inputParametersNames={"Attack", "Release", "Pitch Attack", "Pitch Drop", "Gain"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersDefault={0,.5,0,1,0.5};

enum SynthParams
{
    kAttackParam = 0,
    kReleaseParam,
    kPitchAttackParam,
    kPitchReleaseParam,
    kGainParam,
};

// internal processing properties and functions---------------------
double currentPitchOffset=0;
const double period=2*PI;

double amplitudeAttackCoeff=0;
double amplitudeReleaseCoeff=0;
double omegaCoeff=0;
double omegaReleaseCoeff=0;
double gain=0;
bool pedalIsDown=false;
uint activeVoicesCount=0;
struct SynthVoice;
array<SynthVoice> voices(24);

/** Class that manages a single voice for the synth.
 *
 */
struct SynthVoice
{
    double amplitude=0;
    double omega=0;
    
    double currentAmplitude=0;
    double currentOmega=0;
    double currentPhase=0;
    
    int    currentNote=-1;
    bool   waitingForPedalRelease;
    
    // handle note on operation
    void NoteOn(const MidiEvent& evt)
    {
        amplitude=double(MidiEventUtils::getNoteVelocity(evt))/127.0;
        currentNote=MidiEventUtils::getNote(evt);
        omega=2*PI*pow(2,((double(currentNote-69.0)+currentPitchOffset)/12.0))*440.0/sampleRate;
        
        // force updating currentOmega if omegaReleaseCoeff is 0
        if(omegaReleaseCoeff==0 && currentOmega>omega)
            currentOmega=omega;
    }
    
    void NoteOff()
    {
        if(!pedalIsDown)
        {
            // set amplitude to zero. Voice will be freed only when amplitude gets close to zero
            amplitude=0;
            // omega set to zero so that it can decrease according to pitch release time
            omega=0;
        }
        else
        {
            waitingForPedalRelease=true;
        }
    }
    
    void ForcePitch()
    {
        omega=2*PI*pow(2,((double(currentNote-69.0)+currentPitchOffset)/12.0))*440.0/sampleRate;
        currentOmega=omega;
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
        // update amplitude and omega
        if(amplitude>currentAmplitude)
            currentAmplitude+=amplitudeAttackCoeff*(amplitude-currentAmplitude);
        else
        {
            currentAmplitude+=amplitudeReleaseCoeff*(amplitude-currentAmplitude);
            if(currentAmplitude<.0001)
            {
                // value below threshold => voice ended
                
                // if not the last voice in the list
                if(!(voices[activeVoicesCount-1]==*this))
                {
                    // swap with last voice in the list
                    *this=voices[activeVoicesCount-1];
                    voices[activeVoicesCount-1].CancelNote();
                    
                    // decrease voices count
                    activeVoicesCount--;
                    
                    // process swaped voice instead of this one
                    return ProcessSample();
                }
                else
                {
                    // forget current note
                    CancelNote();
                    
                    // decrease voices count
                    activeVoicesCount--;
                    return 0;
                }
            }
        }
        
        if(omega>currentOmega)
            currentOmega+=omegaCoeff*(omega-currentOmega);
        else
            currentOmega+=omegaReleaseCoeff*(omega-currentOmega);
        
        // compute sample value
        sampleValue=currentAmplitude*sin(currentPhase);
        
        // update phase
        currentPhase+=currentOmega;
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
    bool operator==(const SynthVoice& voice)const
    {
        return currentNote==voice.currentNote;
    }
};
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
        default:
            break;
    }
}

DSP_EXPORT void processBlock(BlockData& data)
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

        double sampleValue=0;
        for(uint v=0;v<activeVoicesCount;v++)
        {
            sampleValue+=voices[v].ProcessSample();
        }
        sampleValue*=gain;

        // copy to all outputs
        for(uint ch=0;ch<audioOutputsCount;ch++)
        {
            data.samples[ch][i]=sampleValue;
        }

        // update gain
        gain*=gainRatio;
    }

    // to avoid overflow
    for(uint i=0;i<activeVoicesCount;i++)
    {
        voices[i].ReducePhase();
    }
}

DSP_EXPORT void updateInputParametersForBlock(const TransportInfo* info)
{
    amplitudeAttackCoeff=pow(10,1.0/(50+sampleRate*inputParameters[kAttackParam]))-1;
    amplitudeReleaseCoeff=pow(10,1.0/(50+sampleRate*inputParameters[kReleaseParam]))-1;
    omegaCoeff=pow(10,1.0/(10+.15*sampleRate*inputParameters[kPitchAttackParam]))-1;
    if(inputParameters[kPitchReleaseParam]<.99)
        omegaReleaseCoeff=pow(10,1.0/(1000+10*sampleRate*inputParameters[kPitchReleaseParam]))-1;
    else
        omegaReleaseCoeff=0;
    gain=pow(10,-1+inputParameters[kGainParam]*2);
}

DSP_EXPORT int getTailSize()
{
    return -1;
}