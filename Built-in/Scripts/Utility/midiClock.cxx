/** \file
*   MIDI clock.
*   Send midi clock events to sync external devices with current host. 
*/
#include "../library/Midi.hxx"

//metadata
string name="MIDI Clock";
string description="Send midi clock & start/stop events to sync external devices";

// parameters definition
array<string> inputParametersNames={"Multiplier","Reset On Play"};
array<double> inputParameters(inputParametersNames.length,0);
//
array<double> inputParametersMin={-9,0};
array<double> inputParametersMax={9,1};
array<double> inputParametersDefault={0,0};
array<int>    inputParametersSteps = { 19,2 };
array<string> inputParametersEnums = { "1/10;1/9;1/8;1/7;1/6;1/5/;1/4;1/3;1/2;1;2x;3x;4x;5x;6x;7x;8x;9x;10x", "Off; On"};

// internal variables
MidiEvent   tempEvent;
bool playing = false;
bool resetOnPlay=false;
double lastMidiClockTimeStamp=0;
double nextPositionInQuarterNotes=0;
double multiplier = 1;

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
}

void processBlock(BlockData& data)
{
    // cannot run if no transport available
    if(@data.transport!=null)
    {
        // check if was playing
        bool wasPlaying=playing;
        playing = data.transport.isPlaying;

        // Midi Clock interval is 1/24th of a quarter note
        double timeSigBottom = double(data.transport.timeSigBottom);
        double midiClockIntervalInSamples = multiplier*quarterNotesToSamples(1.0 / 24.0, data.transport.bpm);
        // adjust BPM to QPM
        midiClockIntervalInSamples *= timeSigBottom/4.0;
        double nextTimeStamp = lastMidiClockTimeStamp;
        while (nextTimeStamp < 0)
            nextTimeStamp += midiClockIntervalInSamples;

        // check if transport moved => stop and restart to keep in sync
        bool transportMoved = (data.transport.positionInQuarterNotes >nextPositionInQuarterNotes+1)||(data.transport.positionInQuarterNotes < nextPositionInQuarterNotes - 1);

        // send continue and stop events when transport changes
        if ((!playing && wasPlaying) || transportMoved) // stop immediatly
        {
            tempEvent.timeStamp = 0;
            MidiEventUtils::setType(tempEvent, kStop);
            data.outputMidiEvents.push(tempEvent);
        }

        // position & continue
        if (playing && (!wasPlaying||transportMoved))
        {
            if (resetOnPlay && !wasPlaying)
                nextTimeStamp = 0;// reset clock to beginning of buffer

            // send current position (adjusted to next clock event) at beginning of frame
            tempEvent.timeStamp = 0;

            MidiEventUtils::setType(tempEvent, kSongPointer);
            double positionInQuarterNotes = data.transport.positionInQuarterNotes;
            positionInQuarterNotes += samplesToQuarterNotes(nextTimeStamp, data.transport.bpm);
            if (timeSigBottom != 0)
                positionInQuarterNotes*=4.0 / timeSigBottom;
            int sspPosition = int(positionInQuarterNotes * 24.0 / 6.0);
            MidiEventUtils::setSongPointerPosition(tempEvent,sspPosition);
            data.outputMidiEvents.push(tempEvent);

            // send continue event
            MidiEventUtils::setType(tempEvent, kContinue);
            data.outputMidiEvents.push(tempEvent);
       }

        // always send midi clock events periodically, even if not playing
        while(nextTimeStamp<double(data.samplesToProcess))
        {
            tempEvent.timeStamp = nextTimeStamp;
            MidiEventUtils::setType(tempEvent, kBeatClock);
            data.outputMidiEvents.push(tempEvent);
            lastMidiClockTimeStamp = nextTimeStamp;
            nextTimeStamp = lastMidiClockTimeStamp+midiClockIntervalInSamples;
        }

        // update time stamps for next frame
        lastMidiClockTimeStamp -= double(data.samplesToProcess);
        nextPositionInQuarterNotes = data.transport.positionInQuarterNotes + samplesToQuarterNotes(data.samplesToProcess, data.transport.bpm) * timeSigBottom / 4.0;
    }
}

void updateInputParametersForBlock(const TransportInfo@ )
{
    multiplier=inputParameters[0];
    if (multiplier < 0)
        multiplier = floor(-multiplier + 1.5);
    else
        multiplier = 1.0/floor(1.5 + multiplier);
    resetOnPlay = inputParameters[1] > .5;
}