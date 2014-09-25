#include "../library/Midi.hxx"

// metadata
string name="MIDI Velocity Randomizer";
string description="Randomizes MIDI Note velocity by the selected amount";

// parameters definition
array<string> inputParametersNames={"Randomize"};
array<double> inputParameters(inputParametersNames.length);

// temp MIDI event
MidiEvent tempEvent;

void processBlock(BlockData& data)
{
    double randomMix=inputParameters[0];
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // randomize velocity for Note On and Off events
        MidiEventType type=MidiEventUtils::getType(data.inputMidiEvents[i]);
        int8 velocity=MidiEventUtils::getNoteVelocity(data.inputMidiEvents[i]);
        if(type==kMidiNoteOn && velocity>0) // 0 is often used for note offs so we won't randomize it
        {
            tempEvent=data.inputMidiEvents[i];
            double newVelocity=randomMix*rand(0,127)+(1-randomMix)*double(velocity);
            MidiEventUtils::setNoteVelocity(tempEvent,int8(newVelocity+.5));
            data.outputMidiEvents.push(tempEvent);
        }
        else
        {
            // forward other events (unchanged)
            data.outputMidiEvents.push(data.inputMidiEvents[i]);
        }
    }
}