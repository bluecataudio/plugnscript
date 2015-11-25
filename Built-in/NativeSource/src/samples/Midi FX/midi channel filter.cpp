// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

/** \file
*   MIDI channel filter.
*   Filter MIDI events (keeps only MIDI events for the selected channel).
*/

#include "../library/Midi.h"

// metadata
DSP_EXPORT string name="MIDI Channel Filter";
DSP_EXPORT string description="Filters events on channel (use 0 for omni)";

// parameters
DSP_EXPORT array<string> inputParametersNames={"Channel"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);

DSP_EXPORT array<double> inputParametersMin={0};
DSP_EXPORT array<double> inputParametersMax={16};
DSP_EXPORT array<double> inputParametersDefault={0};
DSP_EXPORT array<int> inputParametersSteps={17};

/* per-block processing function: called for every block with updated parameters values.
*
*/
DSP_EXPORT void processBlock(BlockData& data)
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