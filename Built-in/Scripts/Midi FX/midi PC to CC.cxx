/** \file
*   MIDI PC to CC, with optional channel filter.
*   Changes Program Change MIDI events into Control Change events (only for MIDI events with the selected channel if non-zero).
*/

#include "./library/Midi.hxx"

// metadata
string name="MIDI PC to CC Converter";
string description="Changes PC events into CC for selected channel (use 0 for omni)";

// parameters
array<string> inputParametersNames={"Channel", "CC#"};
array<double> inputParameters(inputParametersNames.length);

array<double> inputParametersMin={0,0};
array<double> inputParametersMax={16,127};
array<double> inputParametersDefault={0,7};
array<int> inputParametersSteps={17,128};
array<string> inputParametersFormats={".0",".0"};

// local variables
MidiEvent tempEvent; ///< defining temp object in the script to avoid allocations in time-critical processBlock function

/* per-block processing function: called for every block with updated parameters values.
*
*/
void processBlock(BlockData& data)
{
    // read parameters & init temp event
    uint8 channel=int8(inputParameters[0]+.5);
    uint8 ccNumber=int8(inputParameters[1]+.5);
    MidiEventUtils::setType(tempEvent,kMidiControlChange);
    MidiEventUtils::setCCNumber(tempEvent,ccNumber);

    // iterate on MIDI events
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // modify only events with appropriate channel and type
        uint8 eventChannel=MidiEventUtils::getChannel(data.inputMidiEvents[i]);
        if((eventChannel==channel || channel==0) && (MidiEventUtils::getType(data.inputMidiEvents[i])==kMidiProgramChange))
        {
	    // Convert to MIDI CC
            MidiEventUtils::setCCValue(tempEvent,MidiEventUtils::getProgram(data.inputMidiEvents[i]));
            data.outputMidiEvents.push(tempEvent);
	}
	else
	{
            // forward the event (unchanged)
            data.outputMidiEvents.push(data.inputMidiEvents[i]);
        }
    }
}