/** \file
*   MIDI log.
*   Write the content of MIDI events to the log file.
*/

#include "../library/Midi.hxx"

// internal data---------------------------
array<string> MidiEventsTypes(kUnknown+1,"-Event-");
MidiEvent tempEvent;
string consoleMessage;
string temp;
double positionInSeconds=0;

// internal functions----------------------
void printEvent(const MidiEvent& evt)
{
    MidiEventType type=MidiEventUtils::getType(evt);
    consoleMessage=MidiEventsTypes[type];
    switch(type)
    {
    case kMidiNoteOn:
    case kMidiNoteOff:
        {
            consoleMessage+=" " + MidiEventUtils::getNote(evt);
            consoleMessage+=" Vel: " + MidiEventUtils::getNoteVelocity(evt);
            consoleMessage += " on Ch.";
            consoleMessage += MidiEventUtils::getChannel(evt);
            break;
        }
    case kMidiControlChange:
        {
            consoleMessage+=" " + MidiEventUtils::getCCNumber(evt);
            consoleMessage+=" Val: " + MidiEventUtils::getCCValue(evt);
            consoleMessage += " on Ch.";
            consoleMessage += MidiEventUtils::getChannel(evt);
            break;
        }
    case kMidiProgramChange:
        {
            consoleMessage+=" " + MidiEventUtils::getProgram(evt);
            consoleMessage += " on Ch.";
            consoleMessage += MidiEventUtils::getChannel(evt);
            break;
        }
    case kMidiPitchWheel:
        {
            consoleMessage+=" " + MidiEventUtils::getPitchWheelValue(evt);
            consoleMessage += " on Ch.";
            consoleMessage += MidiEventUtils::getChannel(evt);
            break;
        }
    case kMidiChannelAfterTouch:
        {
            consoleMessage+=" Value: " + MidiEventUtils::getChannelAfterTouchValue(evt);
            consoleMessage += " on Ch.";
            consoleMessage += MidiEventUtils::getChannel(evt);
            break;
        }
    case kMidiNoteAfterTouch:
        {
            consoleMessage+=" " + MidiEventUtils::getNote(evt);
            consoleMessage+=" Value: " + MidiEventUtils::getNoteVelocity(evt);
            consoleMessage += " on Ch.";
            consoleMessage += MidiEventUtils::getChannel(evt);
            break;
        }
    case kSongPointer:
        {
            consoleMessage += " " + MidiEventUtils::getSongPointerPosition(evt);
        }
    }
    consoleMessage+=" (";
    consoleMessage+= formatInt(evt.byte0, "0H", 2);
    consoleMessage+="/";
    consoleMessage+= formatInt(evt.byte1, "0H", 2);
    consoleMessage+="/";
    consoleMessage+= formatInt(evt.byte2, "0H", 2);
    if (MidiEventUtils::isSysexData(evt))
    {
        consoleMessage += "/";
        consoleMessage += formatInt(evt.byte3, "0H", 2);
    }
    consoleMessage+=")";
    consoleMessage += " - ";
    consoleMessage += (evt.timeStamp) / sampleRate + positionInSeconds;
    consoleMessage += "s";
    print(consoleMessage);
}

// filter interface-------------------------
string name="MIDI Log";
string description="Logs incoming MIDI events";

void initialize()
{
    MidiEventsTypes[kMidiNoteOff]="Note Off";
    MidiEventsTypes[kMidiNoteOn]="Note On ";
    MidiEventsTypes[kMidiControlChange]="Control Change";
    MidiEventsTypes[kMidiProgramChange]="Program Change";
    MidiEventsTypes[kMidiPitchWheel]="Pitch Wheel";
    MidiEventsTypes[kMidiNoteAfterTouch]="Note After Touch";
    MidiEventsTypes[kMidiChannelAfterTouch]="Channel After Touch";
    MidiEventsTypes[kSysexStart] = "SysEx Start";
    MidiEventsTypes[kSysexEnd] = "SysEx End";
    MidiEventsTypes[kBeatClock] = "Beat Clock";
    MidiEventsTypes[kStart] = "Start";
    MidiEventsTypes[kSongPointer] = "Song Pointer Position";
    MidiEventsTypes[kContinue] = "Continue";
    MidiEventsTypes[kStop] = "Stop";
    MidiEventsTypes[kUnknown] = "Bytes";
}

void processBlock(BlockData& data)
{
    if(@data.transport!=null)
        positionInSeconds = data.transport.positionInSeconds;

    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // print event
        printEvent(data.inputMidiEvents[i]);
        // forward event (unchanged)
        data.outputMidiEvents.push(data.inputMidiEvents[i]);
    }
}
