/** \file
*   Simple Monophonic Sine Wave Synthesizer.
*   Probably the simplest possible virtual instrument: a single voice that generates a sine wave, with no envelope control.
*/

#include "../library/Midi.hxx"
#include "../library/Constants.hxx"

// internal processing properties and functions
double amplitude=0;
double omega=0;

double currentAmplitude=0;
double currentPhase=0;
double currentPitchOffset=0;
uint8  currentNote=0;
const double period=2*PI;

void handleMidiEvent(const MidiEvent& evt)
{
    switch(MidiEventUtils::getType(evt))
    {
    case kMidiNoteOn:
        {
            amplitude=double(MidiEventUtils::getNoteVelocity(evt))/127.0;
            currentNote=MidiEventUtils::getNote(evt);
            omega=2*PI*pow(2,((double(currentNote-69.0)+currentPitchOffset)/12.0))*440.0/sampleRate;
            break;
        }
    case kMidiNoteOff:
        {
            if(currentNote==MidiEventUtils::getNote(evt))
                amplitude=0;
            break;
        }
    case kMidiPitchWheel:
        {
            currentPitchOffset=2*double(MidiEventUtils::getPitchWheelValue(evt))/8192.0;
            omega=2*PI*pow(2,((double(currentNote-69.0)+currentPitchOffset)/12.0))*440.0/sampleRate;
            break;
        }
    }
}

string name="Mono Sin Synth";
string description="Simple monophonic sine wave synth";

MidiEvent tempEvent;

/** per-block processing function: called for every sample with updated parameters values.
*
*/
void processBlock(BlockData& data)
{
    uint nextEventIndex=0;

    MidiEventUtils::setType(tempEvent,kMidiPitchWheel);
    for(uint i=0;i<data.samplesToProcess;i++)
    {
        // manage MIDI events
        while(nextEventIndex!=data.inputMidiEvents.length && data.inputMidiEvents[nextEventIndex].timeStamp<=double(i))
        {
            handleMidiEvent(data.inputMidiEvents[nextEventIndex]);
            nextEventIndex++;
        }

        // update amplitude smoothly (to avoid clicks)
        currentAmplitude=.99*currentAmplitude+.01*amplitude;

        // compute sample value
        double sampleValue=currentAmplitude*sin(currentPhase);

        // update phase
        currentPhase+=omega;

        // copy to all audio outputs
        for(uint ch=0;ch<audioOutputsCount;ch++)
        {
            data.samples[ch][i]=sampleValue;
        }
    }

    // to avoid overflow, reduce phase
    while(currentPhase>period)
        currentPhase-=period;
}

int getTailSize()
{
    return -1;
}