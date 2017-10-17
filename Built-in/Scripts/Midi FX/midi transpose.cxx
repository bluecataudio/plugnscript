/** \file
*   MIDI transposer.
*   Shift MIDI Note events by the selected amount (semitones).
*/

#include "../library/Midi.hxx"

// metadata
string name="MIDI Transpose";
string description="Transposes MIDI Note events by the selected amount";

// parameters definition
array<string> inputParametersNames={"Pitch Shift"};
array<double> inputParameters(inputParametersNames.length);

array<double> inputParametersMin={-24};
array<double> inputParametersMax={24};
array<double> inputParametersDefault={0};
array<int> inputParametersSteps={49};
array<string> inputParametersUnits={"st"};
array<string> inputParametersFormats={"+.0"} ;

// temp MIDI events for processing
MidiEvent tempEvent;

/* per-block processing function: called for every sample with updated parameters values.
*
*/
void processBlock(BlockData& data)
{
    // store offset as integer
    int8 offset=int8(inputParameters[0]+.5);
    if(offset<0)
        offset-=1;

    // Read incoming MIDI events
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // transpose Note On and Off events
        MidiEventType type=MidiEventUtils::getType(data.inputMidiEvents[i]);
        if(type==kMidiNoteOn || type==kMidiNoteOff)
        {
            tempEvent=data.inputMidiEvents[i];
            int8 note=MidiEventUtils::getNote(data.inputMidiEvents[i]);
            note+=offset;
            if(note>=0)
            {
                MidiEventUtils::setNote(tempEvent,note);
                data.outputMidiEvents.push(tempEvent);
            }
        }
        else
        {
            // forward other events (unchanged)
            data.outputMidiEvents.push(data.inputMidiEvents[i]);
        }
    }
}