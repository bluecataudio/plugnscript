/** \file
*   MIDI note events filter.
*   Filters MIDI events and let only MIDI Note On and Off events go thru.
*/
#include "../library/Midi.hxx"

string name="MIDI Range Filter";
string author="Blue Cat Audio";
string description="Filter MIDI note events based on selected range";


// parameters definition
string midiNotes="C-1;C#-1;D;D#-1;E-1;F-1;F#-1;G-1;G#-1;A-1;A#-1;B-1;C0;C#0;D0;D#0;E0;F0;F#0;G0;G#0;A0;A#0;B0;C1;C#1;D1;D#1;E1;F1;F#1;G1;G#1;A1;A#1;B1;C2;C#2;D2;D#2;E2;F2;F#2;G2;G#2;A2;A#2;B2;C3;C#3;D3;D#3;E3;F3;F#3;G3;G#3;A3;A#3;B3;C4;C#4;D4;D#4;E4;F4;F#4;G4;G#4;A4;A#4;B4;C5;C#5;D5;D#5;E5;F5;F#5;G5;G#5;A5;A#5;B5;C6;C#6;D6;D#6;E6;F6;F#6;G6;G#6;A6;A#6;B6;C7;C#7;D7;D#7;E7;F7;F#7;G7;G#7;A7;A#7;B7;C8;C#8;D8;D#8;E8;F8;F#8;G8;G#8;A8;A#8;B8;C9;C#9;D9;D#9;E9;F9;F#9;G9";

array<string> inputParametersNames={"From", "To"};
array<double> inputParameters(inputParametersNames.length,0);

array<double> inputParametersMin={0,0};
array<double> inputParametersMax={127,127};
array<double> inputParametersDefault={0,127};
array<int>    inputParametersSteps={128,128};
array<string> inputParametersEnums={midiNotes,midiNotes};

uint8 minNote=0;
uint8 maxNote=127;

void processBlock(BlockData& data)
{
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        auto@ midiEvent=data.inputMidiEvents[i];
        // Check for Note On and Off events as well as Note Aftertouch
        MidiEventType type=MidiEventUtils::getType(midiEvent);
        if(type==kMidiNoteOn || type==kMidiNoteOff || type==kMidiNoteAfterTouch)
        {
            // forward the event (unchanged) only if in range
            uint8 note=MidiEventUtils::getNote(midiEvent);
            if(note>=minNote && note<=maxNote)
                data.outputMidiEvents.push(midiEvent);
        }
        // forward all other events unchanged
        else
            data.outputMidiEvents.push(midiEvent);
    }
}

void updateInputParametersForBlock(const TransportInfo@ )
{
    minNote=uint8(inputParameters[0]+.5);
    maxNote=uint8(inputParameters[1]+.5);
}