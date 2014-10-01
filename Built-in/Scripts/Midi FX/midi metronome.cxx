/** \file
*   MIDI metronome.
*   Send MIDI note events in sync with the host. 
*/
#include "../library/Midi.hxx"

//metadata
string name="MIDI Metronome";
string description="Sends midi events on beats";

// parameters definition
string midiNotes="C-1;C#-1;D;D#-1;E-1;F-1;F#-1;G-1;G#-1;A-1;A#-1;B-1;C0;C#0;D0;D#0;E0;F0;F#0;G0;G#0;A0;A#0;B0;C1;C#1;D1;D#1;E1;F1;F#1;G1;G#1;A1;A#1;B1;C2;C#2;D2;D#2;E2;F2;F#2;G2;G#2;A2;A#2;B2;C3;C#3;D3;D#3;E3;F3;F#3;G3;G#3;A3;A#3;B3;C4;C#4;D4;D#4;E4;F4;F#4;G4;G#4;A4;A#4;B4;C5;C#5;D5;D#5;E5;F5;F#5;G5;G#5;A5;A#5;B5;C6;C#6;D6;D#6;E6;F6;F#6;G6;G#6;A6;A#6;B6;C7;C#7;D7;D#7;E7;F7;F#7;G7;G#7;A7;A#7;B7;C8;C#8;D8;D#8;E8;F8;F#8;G8;G#8;A8;A#8;B8;C9;C#9;D9;D#9;E9;F9;F#9;G9";

array<string> inputParametersNames={"1st Note", "Note", "1st Velocity" ,"Velocity", "Duration"};
array<double> inputParameters(inputParametersNames.length,0);

array<double> inputParametersMin={0,0,0,0};
array<double> inputParametersMax={127,127,127,127};
array<double> inputParametersDefault={69,50,127,100,.5};
array<int>    inputParametersSteps={128,128,128,128};
array<string> inputParametersEnums={midiNotes,midiNotes};

// internal variables
MidiEvent   noteOnEvent;
MidiEvent   noteOffEvent;
bool        playing=false;

uint8   firstBeatNote=0;
uint8   note=0;
uint8   firstBeatVelocity=0;
uint8   velocity=0;
double  duration=0;

//utils
double quarterNotesToSamples(double position,double bpm)
{
    return position*60.0*sampleRate/bpm;
}
double samplesToQuarterNotes(double samples,double bpm)
{
    return samples*bpm/(60.0*sampleRate);
}

// filter interface starts here---------------------
void initialize()
{
    // initialize our events
    MidiEventUtils::setType(noteOnEvent,kMidiNoteOn);
    MidiEventUtils::setNoteVelocity(noteOnEvent,127);
    noteOffEvent=noteOnEvent;
    MidiEventUtils::setType(noteOffEvent,kMidiNoteOff);
}

void processBlock(BlockData& data)
{
    // check if was playing
    bool wasPlaying=playing;

    // cannot run if no transport available
    if(@data.transport!=null)
    {
        // update note value and send note offs if necessary (cancel runnning notes if changed)
        uint8 newfirstBeatNote=uint8(inputParameters[0]+.5);
        uint8 newNote=uint8(inputParameters[1]+.5);
        if(newfirstBeatNote!=firstBeatNote) // first beat
        {
            if(MidiEventUtils::getNote(noteOnEvent)==firstBeatNote)
                noteOffEvent.timeStamp=0;
            MidiEventUtils::setNote(noteOffEvent,firstBeatNote);
            data.outputMidiEvents.push(noteOffEvent);
            firstBeatNote=newfirstBeatNote;
        }
        if(newNote!=note) //other beats
        {
            if(MidiEventUtils::getNote(noteOnEvent)==note)
                noteOffEvent.timeStamp=0;
            MidiEventUtils::setNote(noteOffEvent,note);
            data.outputMidiEvents.push(noteOffEvent);
            note=newNote;
        }

        // send note off on play stop
        playing=data.transport.isPlaying;
        if(!playing && wasPlaying)
        {
            noteOffEvent.timeStamp=0;
            MidiEventUtils::setNote(noteOffEvent, MidiEventUtils::getNote(noteOnEvent));
            data.outputMidiEvents.push(noteOffEvent);
        }

        // send events when playing
        if(playing && data.transport.timeSigBottom!=0)
        { 
            //convert all values to match current time signature bottom value
            double timeSigBottom=double(data.transport.timeSigBottom);
            double beginPosition=data.transport.positionInQuarterNotes*timeSigBottom/4.0;
            double endPosition=(data.transport.positionInQuarterNotes+samplesToQuarterNotes(data.samplesToProcess,data.transport.bpm))*timeSigBottom/4.0;
            double downBeat=(data.transport.currentMeasureDownBeat*timeSigBottom)/4.0;

            // check where integer position values stand in the buffer
            double beatPosition=floor(beginPosition);
            while(beatPosition<=endPosition)
            {
                int currentPositionInMeasure=int(beatPosition-downBeat+.5);
                bool firstBeat=(currentPositionInMeasure%data.transport.timeSigTop)==0;

                // manage beat note on
                if(beatPosition>=beginPosition)
                {
                    // we have a beat => send midi note on event
                    noteOnEvent.timeStamp=quarterNotesToSamples((beatPosition-beginPosition)*4.0/timeSigBottom,data.transport.bpm);

                    // if it is a first beat, let's use the first beat note
                    if(firstBeat)
                    {
                        MidiEventUtils::setNote(noteOnEvent,firstBeatNote);
                        MidiEventUtils::setNoteVelocity(noteOnEvent,firstBeatVelocity);
                    }
                    else
                    {
                        MidiEventUtils::setNote(noteOnEvent,note);
                        MidiEventUtils::setNoteVelocity(noteOnEvent,velocity);
                    }
                    data.outputMidiEvents.push(noteOnEvent);
                }

                // manage beat note off
                if(beatPosition+duration<=endPosition && beatPosition+duration>=beginPosition)
                {
                    // we have a beat+duration => send midi note off event
                    if(firstBeat)
                    {
                        MidiEventUtils::setNote(noteOffEvent,firstBeatNote);
                        MidiEventUtils::setNoteVelocity(noteOffEvent,firstBeatVelocity);
                    }
                    else
                    {
                        MidiEventUtils::setNote(noteOffEvent,note);
                        MidiEventUtils::setNoteVelocity(noteOffEvent,velocity);
                    }
                    noteOffEvent.timeStamp=quarterNotesToSamples((beatPosition+duration-beginPosition)*4.0/timeSigBottom,data.transport.bpm);
                    data.outputMidiEvents.push(noteOffEvent);
                } 
                beatPosition++;
            }
        }
    }
}

void updateInputParametersForBlock(const TransportInfo@ )
{
    firstBeatVelocity=uint8(inputParameters[2]+.5);
    velocity=uint8(inputParameters[3]+.5);
    duration=inputParameters[4];
}