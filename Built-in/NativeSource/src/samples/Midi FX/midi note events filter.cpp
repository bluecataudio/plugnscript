// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

/** \file
*   MIDI note events filter.
*   Filters MIDI events and let only MIDI Note On and Off events go thru.
*/
#include "../library/Midi.h"

DSP_EXPORT string name="MIDI Notes Filter";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Only keeps note events";

DSP_EXPORT void processBlock(BlockData& data)
{
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // send only Note On and Off events
        MidiEventType type=MidiEventUtils::getType(data.inputMidiEvents[i]);
        if(type==kMidiNoteOn || type==kMidiNoteOff)
        {
            // forward the event (unchanged)
            data.outputMidiEvents.push(data.inputMidiEvents[i]);
        }
    }
}