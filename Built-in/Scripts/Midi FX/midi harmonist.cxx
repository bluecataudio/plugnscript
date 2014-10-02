/** \file
*   MIDI harmonizer.
*   Adds transposed MIDI events based on selected major key.
*/

#include "../library/Midi.hxx"
#include "../library/Utils.hxx"

// metadata
string name="MIDI Harmonizer";
string description="Adds transposed MIDI events based on selected major key";

// parameters
array<string> inputParametersNames={"Key", "Note 1", "Note 2", "Note 3", "Note 4"} ;
array<double> inputParameters(inputParametersNames.length);

string intervals="-11th;-9th;-Oct;-7th;-6th;-5th;-4th;-3rd;-2nd;None;+2nd;+3rd;+4th;+5th;+6th;+7th;+Oct;+9th;+11th";
array<double> inputParametersMin={0,-9,-9,-9,-9};
array<double> inputParametersMax={11,9,9,9,9};
array<double> inputParametersDefault={0,0,0,0,0};
array<string>  inputParametersEnums={"C;C#;D;D#;E;F;F#;G;G#;A;A#;B",intervals,intervals,intervals,intervals};
array<int>    inputParametersSteps={12,19,19,19,19};

// internal variables-------------------
// list of notes in the major key
const array<int> majorKeyNotes={0,2,4,5,7,9,11};
// predefined intervals for notes that are not in the scale
const array<int> predefinedIntervals={-16,-14,-12,-11,-8,-7,-5,-4,-2,0,2,4,5,7,8,11,12,14,16};

array<int8> offsets(4);
uint8 key=0;
MidiEvent tempEvent;

int getRelativeNoteValueInScale(int index)
{
    // make note index positive
    while(index<0)
        index+=(majorKeyNotes.length);

    // modulo so that it is kept in range
    index%=(majorKeyNotes.length);
    return majorKeyNotes[index];
}

int sign(int i)
{
    if(i>=0)
        return 1;
    else
        return -1;
}
void processBlock(BlockData& data)
{
    // iterate on MIDI events
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // forward all events (unchanged)
        data.outputMidiEvents.push(data.inputMidiEvents[i]);

        // add transposed Note On and Off events
        MidiEventType type=MidiEventUtils::getType(data.inputMidiEvents[i]);
        if(type==kMidiNoteOn || type==kMidiNoteOff)
        {
            // determine the index of the note in the major scale
            int absoluteNoteInScale=MidiEventUtils::getNote(data.inputMidiEvents[i])-key;
            int noteInScale=(absoluteNoteInScale%12);
            int noteIndexInScale=majorKeyNotes.find(noteInScale%12);
            for(uint j=0;j<offsets.length;j++)
            {
                if(offsets[j]!=0)
                {
                    int diff=0;

                    // if the note has been found, use our scale to find the appropriate interval
                    if(noteIndexInScale>=0)
                    {
                        int newNoteInScale=getRelativeNoteValueInScale(noteIndexInScale+offsets[j]);
                        diff=newNoteInScale-noteInScale;
                        // set proper octave
                        if(diff*offsets[j]<0)
                        {
                            diff+=12*sign(offsets[j]);
                        }
                        // handle case for more than one octave
                        diff+=(offsets[j]/7)*12;
                    }
                    else
                    {
                        // use predefined intervals (note is not in the scale anyway)
                        diff=predefinedIntervals[offsets[j]+9];
                    }

                    // create new note event if valid (not out of MIDI range)
                    int newNote=MidiEventUtils::getNote(data.inputMidiEvents[i])+diff;
                    if(newNote>=0 && newNote<128)
                    {
                        tempEvent=data.inputMidiEvents[i];
                        MidiEventUtils::setNote(tempEvent,newNote);
                        data.outputMidiEvents.push(tempEvent);
                    }
                }
            }
        }
    }
}

// Update our internal variables from input Parameters values
void updateInputParametersForBlock(const TransportInfo@ info)
{
    key=uint8(inputParameters[0]+.5);
    for(uint i=0;i<offsets.length;i++)
    {
        offsets[i]=KittyDSP::Utils::roundDoubleToInt(inputParameters[i+1]);
    }
}
