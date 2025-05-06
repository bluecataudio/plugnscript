/** \file
*   MIDI clock.
*   Send midi clock events to sync external devices with current host. 
*/
#include "../library/Midi.hxx"

//metadata
string name="MIDI Time Code (MTC)";
string description="Send MTC signal & start/stop events to sync external devices";

// parameters definition
array<string> inputParametersNames={"Frame Rate"};
array<double> inputParameters(inputParametersNames.length,0);
//
array<double> inputParametersMin={0};
array<double> inputParametersMax={3};
array<double> inputParametersDefault={0};
array<int>    inputParametersSteps = { 4};
array<string> inputParametersEnums = { "24;25;29.97;30", "Off; On"};
array<string> inputParametersUnits = { "Fps"};

enum MTCFrameRate
{
    FR_24 = 0x0,
    FR_25 = 0x1,
    FR_29 = 0x2,
    FR_30 = 0x3
};

// internal variables
MidiEvent   tempEvent;
bool playing = false;
double lastMTCTimeStamp=0;
double nextPositionInSamples=0;
double frameRate = 30;
int frameRateType = FR_30;
uint8 currentMessageIndex=0;

uint8 roundUint8(double d)
{
    if (d > 0)
        return uint8(d + .5);
    else
        return uint8(d - .5);
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

        // MTC interval is 1/4th of frameRate
        double mtcIntervalInSamples = sampleRate/frameRate/4.0;
        double nextTimeStamp = lastMTCTimeStamp;
        while (nextTimeStamp < 0)
            nextTimeStamp += mtcIntervalInSamples;

        // check if transport moved => stop and restart to keep in sync
        bool transportMoved = (data.transport.positionInSamples > nextPositionInSamples+data.samplesToProcess)||(data.transport.positionInSamples < nextPositionInSamples - data.samplesToProcess);

        // send position if start playing or if transport moved
        if ((playing && !wasPlaying)||transportMoved)
        {
            if (!wasPlaying)
                nextTimeStamp = 0; // reset clock to beginning of buffer

            // send current position (adjusted to next clock event) at beginning of frame
            tempEvent.timeStamp = 0;

            // compute HH:MM:SS value from position in seconds
            double positionInSeconds = data.transport.positionInSeconds;
            positionInSeconds += nextTimeStamp/sampleRate;
            uint8 hh = uint8(floor(positionInSeconds/3600.0));
            positionInSeconds -= double(hh * 3600);
            uint8 mm = uint8(positionInSeconds/60.0);
            positionInSeconds -= double(mm*60);
            uint8 ss = uint8(positionInSeconds);
            positionInSeconds -= double(ss);
            uint8 ff = roundUint8(positionInSeconds * frameRate);

            // send Full frame sysex event
            tempEvent.byte0 = 0xF0; // Sysex Start
            tempEvent.byte1 = 0x7F; // Real time MIDI
            tempEvent.byte2 = 0x7F; // Broadcast
            tempEvent.byte3 = 0x01; // Timecode Message
            data.outputMidiEvents.push(tempEvent);
            tempEvent.byte0 = 0x01; // full frame time code
            tempEvent.byte1 = ((hh) & 0x1F) | ((frameRateType << 5) & 0x60); // type & hours
            tempEvent.byte2 = mm;
            tempEvent.byte3 = ss;
            data.outputMidiEvents.push(tempEvent);
            tempEvent.byte0 = ff; // frames
            tempEvent.byte1 = 0xF7; // end sysex
            data.outputMidiEvents.push(tempEvent);

            // init message index for quarter frames
            currentMessageIndex = 0;
       }

        // send quarter frame events while playing
        if (playing)
        {
            while (nextTimeStamp<double(data.samplesToProcess))
            {
                double positionInSeconds = data.transport.positionInSeconds;
                positionInSeconds += nextTimeStamp / sampleRate;
                uint8 hh = uint8(floor(positionInSeconds / 3600.0));
                positionInSeconds -= double(hh * 3600);
                uint8 mm = uint8(positionInSeconds / 60.0);
                positionInSeconds -= double(mm * 60);
                uint8 ss = uint8(positionInSeconds);
                positionInSeconds -= double(ss);
                uint8 ff = roundUint8(positionInSeconds * frameRate);

                // Quarter Frame events follow each other with different types of information
                tempEvent.timeStamp = nextTimeStamp;
                tempEvent.byte0 = 0xF1;
                tempEvent.byte1 = (currentMessageIndex & 0x07) << 4; // message type, then data
                switch (currentMessageIndex)
                {
                case 0: // ff lsb
                    tempEvent.byte1 |= (ff & 0xF);
                    break;
                case 1: // ff msb
                    tempEvent.byte1 |= ((ff >> 4) & 0x01);
                    break;
                case 2: // secs lsb
                    tempEvent.byte1 |= (ss & 0xF);
                    break;
                case 3: // secs msb
                    tempEvent.byte1 |= ((ss >> 4) & 0x03);
                    break;
                case 4: // min lsb
                    tempEvent.byte1 |= (mm & 0xF);
                    break;
                case 5: // min msb
                    tempEvent.byte1 |= ((mm >> 4) & 0x03);
                    break;
                case 6: // hh lsb
                    tempEvent.byte1 |= (hh & 0xF);
                    break;
                case 7: // min msb + rate
                    tempEvent.byte1 |= ((hh >> 4) & 0x01);
                    tempEvent.byte1 |= (frameRateType & 0x03) << 1;
                    break;

                }
                tempEvent.byte2 = 0; // not used
                tempEvent.byte3 = 0;// not used
                data.outputMidiEvents.push(tempEvent);

                currentMessageIndex++;
                currentMessageIndex &= 7; // 0 to 7 cycle
                lastMTCTimeStamp = nextTimeStamp;
                nextTimeStamp = lastMTCTimeStamp + mtcIntervalInSamples;
            }

            // update time stamps for next frame
            lastMTCTimeStamp -= double(data.samplesToProcess);
            nextPositionInSamples = data.transport.positionInSamples + data.samplesToProcess;
        }
        else
            nextPositionInSamples = data.transport.positionInSamples;
    }
}

void updateInputParametersForBlock(const TransportInfo@ )
{
    frameRateType = uint8(floor(inputParameters[0]+.5));
    switch (frameRateType)
    {
    case FR_24:
        frameRate = 24;
        break;
    case FR_25:
        frameRate = 25;
        break;
    case FR_29:
        frameRate = 29.97;
        break;
    case FR_30:
        frameRate = 30;
        break;
    }
}