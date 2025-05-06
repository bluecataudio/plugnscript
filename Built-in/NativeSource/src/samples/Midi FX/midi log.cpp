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

// utility
#include <sstream>
#include <iomanip>
std::string formatIntToHex(int i)
{
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(sizeof(uint8) * 2)
        << std::hex << std::uppercase << i;
    return stream.str();
}

// internal data---------------------------
array<string> MidiEventsTypes(kUnknown + 1, "-Event-");
MidiEvent tempEvent;
std::string consoleMessage;
string temp;
double positionInSeconds = 0;
DSP_EXPORT double sampleRate=0;

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
        consoleMessage += " on Ch.";
        consoleMessage += std::to_string(MidiEventUtils::getChannel(evt));
        break;
    }
    case kMidiControlChange:
    {
        consoleMessage += " " + std::to_string(MidiEventUtils::getCCNumber(evt));
        consoleMessage += " Val: " + std::to_string(MidiEventUtils::getCCValue(evt));
        consoleMessage += " on Ch.";
        consoleMessage += std::to_string(MidiEventUtils::getChannel(evt));
        break;
    }
    case kMidiProgramChange:
    {
        consoleMessage += " " + std::to_string(MidiEventUtils::getProgram(evt));
        consoleMessage += " on Ch.";
        consoleMessage += std::to_string(MidiEventUtils::getChannel(evt));
        break;
    }
    case kMidiPitchWheel:
    {
        consoleMessage += " " + std::to_string(MidiEventUtils::getPitchWheelValue(evt));
        consoleMessage += " on Ch.";
        consoleMessage += std::to_string(MidiEventUtils::getChannel(evt));
        break;
    }
    case kMidiChannelAfterTouch:
    {
        consoleMessage += " Value: " + std::to_string(MidiEventUtils::getChannelAfterTouchValue(evt));
        consoleMessage += " on Ch.";
        consoleMessage += std::to_string(MidiEventUtils::getChannel(evt));
        break;
    }
    case kMidiNoteAfterTouch:
    {
        consoleMessage += " " + std::to_string(MidiEventUtils::getNote(evt));
        consoleMessage += " Value: " + std::to_string(MidiEventUtils::getNoteVelocity(evt));
        consoleMessage += " on Ch.";
        consoleMessage += std::to_string(MidiEventUtils::getChannel(evt));
        break;
    }
    case kSongPointer:
    {
        consoleMessage += " " + std::to_string(MidiEventUtils::getSongPointerPosition(evt));
    }
    }
    consoleMessage += " (";
    consoleMessage += formatIntToHex(evt.byte0);
    consoleMessage += "/";
    consoleMessage += formatIntToHex(evt.byte1);
    consoleMessage += "/";
    consoleMessage += formatIntToHex(evt.byte2);
    if (MidiEventUtils::isSysexData(evt))
    {
        consoleMessage += "/";
        consoleMessage += formatIntToHex(evt.byte3);
    }
    consoleMessage += ")";
    consoleMessage += " - ";
    consoleMessage += std::to_string((evt.timeStamp) / sampleRate + positionInSeconds);
    consoleMessage += "s";
    print(consoleMessage.c_str());
}

// filter interface-------------------------
DSP_EXPORT string name="MIDI Log";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Writes the content of MIDI events to the log file";

DSP_EXPORT bool initialize()
{
    MidiEventsTypes[kMidiNoteOff] = "Note Off";
    MidiEventsTypes[kMidiNoteOn] = "Note On ";
    MidiEventsTypes[kMidiControlChange] = "Control Change";
    MidiEventsTypes[kMidiProgramChange] = "Program Change";
    MidiEventsTypes[kMidiPitchWheel] = "Pitch Wheel";
    MidiEventsTypes[kMidiNoteAfterTouch] = "Note After Touch";
    MidiEventsTypes[kMidiChannelAfterTouch] = "Channel After Touch";
    MidiEventsTypes[kSysexStart] = "SysEx Start";
    MidiEventsTypes[kSysexEnd] = "SysEx End";
    MidiEventsTypes[kBeatClock] = "Beat Clock";
    MidiEventsTypes[kStart] = "Start";
    MidiEventsTypes[kSongPointer] = "Song Pointer Position";
    MidiEventsTypes[kContinue] = "Continue";
    MidiEventsTypes[kStop] = "Stop";
    MidiEventsTypes[kUnknown] = "Bytes";
    return true;
}

DSP_EXPORT void processBlock(BlockData& data)
{
    if (data.transport != null)
        positionInSeconds = data.transport->positionInSeconds;

    for (uint i = 0; i < data.inputMidiEvents.length; i++)
    {
        // print event
        printEvent(data.inputMidiEvents[i]);
        // forward event (unchanged)
        data.outputMidiEvents.push(data.inputMidiEvents[i]);
    }
}
