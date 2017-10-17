// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"
#include <string>

DSP_EXPORT void*   host=null;
DSP_EXPORT HostPrintFunc* hostPrint=null;

/** \file
*   MIDI log.
*   Writes the content of MIDI events to the log file.
*/

#include "../library/Midi.h"

// internal data---------------------------
array<string> MidiEventsTypes(kUnknown+1,"-Event-");
MidiEvent   tempEvent;
std::string consoleMessage;

// internal functions----------------------
void printEvent(const MidiEvent& evt)
{
    MidiEventType type = MidiEventUtils::getType(evt);
    consoleMessage = MidiEventsTypes[type];
    switch (type)
    {
        case kMidiNoteOn:
        case kMidiNoteOff:
        {
            consoleMessage += " " + std::to_string(MidiEventUtils::getNote(evt));
            consoleMessage += " Vel: " + std::to_string(MidiEventUtils::getNoteVelocity(evt));
            break;
        }
        case kMidiControlChange:
        {
            consoleMessage += " " + std::to_string(MidiEventUtils::getCCNumber(evt));
            consoleMessage += " Val: " + std::to_string(MidiEventUtils::getCCValue(evt));
            break;
        }
        case kMidiProgramChange:
        {
            consoleMessage += " " + std::to_string(MidiEventUtils::getProgram(evt));
            break;
        }
        case kMidiPitchWheel:
        {
            consoleMessage += " " + std::to_string(MidiEventUtils::getPitchWheelValue(evt));
            break;
        }
        case kMidiChannelAfterTouch:
        {
            consoleMessage += " Value: " + std::to_string(MidiEventUtils::getChannelAfterTouchValue(evt));
            break;
        }
        case kMidiNoteAfterTouch:
        {
            consoleMessage += " " + std::to_string(MidiEventUtils::getNote(evt));
            consoleMessage += " Value: " + std::to_string(MidiEventUtils::getNoteVelocity(evt));
            break;
        }
        default:
            break;
    }
    consoleMessage+=" on Ch.";
    consoleMessage+=std::to_string(MidiEventUtils::getChannel(evt));
    consoleMessage+=" (";
    consoleMessage+=std::to_string(evt.byte0);
    consoleMessage+="/";
    consoleMessage+=std::to_string(evt.byte1);
    consoleMessage+="/";
    consoleMessage+=std::to_string(evt.byte2);
    consoleMessage+=")";
    print(consoleMessage.c_str());
}

// filter interface-------------------------
DSP_EXPORT string name="MIDI Log";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Writes the content of MIDI events to the log file";

DSP_EXPORT bool initialize()
{
    MidiEventsTypes[kMidiNoteOff]="Note Off";
    MidiEventsTypes[kMidiNoteOn]="Note On ";
    MidiEventsTypes[kMidiControlChange]="Control Change";
    MidiEventsTypes[kMidiProgramChange]="Program Change";
    MidiEventsTypes[kMidiPitchWheel]="Pitch Wheel";
    MidiEventsTypes[kMidiNoteAfterTouch]="Note After Touch";
    MidiEventsTypes[kMidiChannelAfterTouch]="Channel After Touch";
    return true;
}

DSP_EXPORT void processBlock(BlockData& data)
{
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // print event
        printEvent(data.inputMidiEvents[i]);
        // forward event (unchanged)
        data.outputMidiEvents.push(data.inputMidiEvents[i]);
    }
}
