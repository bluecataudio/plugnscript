/** \file
*   MIDI channel filter.
*   Filter MIDI events (keeps only MIDI events for the selected channel).
*/

#include "../library/Midi.hxx"

// metadata
string name="MIDI Channel Filter";
string description="Filters events on channel (use 0 for omni)";

// parameters
array<string> inputParametersNames={"Channel"};
array<double> inputParameters(inputParametersNames.length);

array<double> inputParametersMin={0};
array<double> inputParametersMax={16};
array<double> inputParametersDefault={0};
array<int> inputParametersSteps={17};
array<string> inputParametersFormats={".0"};

/* per-block processing function: called for every block with updated parameters values.
*
*/
void processBlock(BlockData& data)
{
    // read parameters
    uint8 channel=int8(inputParameters[0]+.5);

    // iterate on MIDI events
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // send only events with appropriate channel
        uint8 eventChannel=MidiEventUtils::getChannel(data.inputMidiEvents[i]);
        if(eventChannel==channel || channel==0)
        {
            // forward the event (unchanged)
            data.outputMidiEvents.push(data.inputMidiEvents[i]);
        }
    }
}