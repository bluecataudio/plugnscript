/** \file
*   MIDI channel changer.
*   MIDI effect that lets you change the MIDI channel of events.
*/

#include "../library/Midi.hxx"

// metadata
string name="MIDI Channel Changer";
string description="Change channel for events (use 0 for omni)";

// parameters
array<string> inputParametersNames={"Source Ch.", "Dest Ch."};
array<double> inputParameters(inputParametersNames.length);

array<double> inputParametersMin={0,1};
array<double> inputParametersMax={16,16};
array<double> inputParametersDefault={0,1};
array<int>    inputParametersSteps={17,16};
array<string> inputParametersFormats={".0", ".0"} ;

// local variables
MidiEvent tempEvent; ///< defining temp object in the script to avoid allocations in time-critical processBlock function

/* per-block processing function: called for every block with updated parameters values.
*
*/
void processBlock(BlockData& data)
{
    // reading parameters
    uint8 sourceChannel=int8(inputParameters[0]+.5);
    uint8 destChannel=int8(inputParameters[1]+.5);

    // iterating on MIDI events
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // send only events with appropriate channel
        uint8 eventChannel=MidiEventUtils::getChannel(data.inputMidiEvents[i]);
        if(eventChannel==sourceChannel || sourceChannel==0)
        {
            // forward the event on the new channel
            tempEvent=data.inputMidiEvents[i];
            MidiEventUtils::setChannel(tempEvent,destChannel);
            data.outputMidiEvents.push(tempEvent);
        }
    }
}