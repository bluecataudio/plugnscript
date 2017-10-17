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
            break;
        }
    case kMidiControlChange:
        {
            consoleMessage+=" " + MidiEventUtils::getCCNumber(evt);
            consoleMessage+=" Val: " + MidiEventUtils::getCCValue(evt);
            break;
        }
    case kMidiProgramChange:
        {
            consoleMessage+=" " + MidiEventUtils::getProgram(evt);
            break;
        }
    case kMidiPitchWheel:
        {
            consoleMessage+=" " + MidiEventUtils::getPitchWheelValue(evt);
            break;
        }
    case kMidiChannelAfterTouch:
        {
            consoleMessage+=" Value: " + MidiEventUtils::getChannelAfterTouchValue(evt);
            break;
        }
    case kMidiNoteAfterTouch:
        {
            consoleMessage+=" " + MidiEventUtils::getNote(evt);
            consoleMessage+=" Value: " + MidiEventUtils::getNoteVelocity(evt);
            break;
        }
    }
    consoleMessage+=" on Ch.";
    consoleMessage+=MidiEventUtils::getChannel(evt);;
    consoleMessage+=" (";
    consoleMessage+=evt.byte0;
    consoleMessage+="/";
    consoleMessage+=evt.byte1;
    consoleMessage+="/";
    consoleMessage+=evt.byte2;
    consoleMessage+=")";
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
}

void processBlock(BlockData& data)
{
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // print event
        printEvent(data.inputMidiEvents[i]);
        // forward event (unchanged)
        data.outputMidiEvents.push(data.inputMidiEvents[i]);
    }
}
