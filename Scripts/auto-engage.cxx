/** \file
*   auto engage MIDI CC
*/

#include "./library/Midi.hxx"

// metadata
string name="MIDI Auto-Engage";
string description="Create MIDI CC on/off from moving pedal events";

// parameters
array<string> inputParametersNames={"Channel", "Source CC","Dest CC","Threshold","Delay"};
array<double> inputParameters(inputParametersNames.length);

array<double> inputParametersMin={0,1,1,1,0};
array<double> inputParametersMax={16,127,127,127,2000};
array<double> inputParametersDefault={0,1,7,4,500};
array<int>    inputParametersSteps={17,128,128,128,128};
array<string> inputParametersFormats={".0", ".0", ".0" , ".0" ,".0"};
array<string> inputParametersUnits = { "", "", "" , "" ,"ms" };


double pauseTimeInSamples = 0;
bool enabled = false;
uint8 lastCCvalue=0;

// local variables
MidiEvent tempEvent; ///< defining temp object in the script to avoid allocations in time-critical processBlock function

/* per-block processing function: called for every block with updated parameters values.
*
*/
void processBlock(BlockData& data)
{
    // reading parameters
    uint8 sourceChannel = int8(inputParameters[0]+.5);
    uint8 sourceCC = int8(inputParameters[1] + .5);
    uint8 destCC = int8(inputParameters[2]+.5);
    uint8 threshold = int8(inputParameters[3] + .5);

    // iterating on MIDI events
    for(uint i=0;i<data.inputMidiEvents.length;i++)
    {
        // check MIDI events from incoming channel
        uint8 eventChannel=MidiEventUtils::getChannel(data.inputMidiEvents[i]);
        if((eventChannel==sourceChannel || sourceChannel==0) && (MidiEventUtils::getType(data.inputMidiEvents[i])==kMidiControlChange) && MidiEventUtils::getCCNumber(data.inputMidiEvents[i])==sourceCC)
        {
            // received MIDI CC => no pause
            pauseTimeInSamples = 0;

            // update last cc value, and create ON control event if not enabled yet,
            // and value above threshold
            tempEvent = data.inputMidiEvents[i];
            lastCCvalue = MidiEventUtils::getCCValue(tempEvent);
            if (!enabled && lastCCvalue >= threshold)
            {
                enabled = true;
                MidiEventUtils::setCCNumber(tempEvent, destCC);
                MidiEventUtils::setCCValue(tempEvent, 127);
                data.outputMidiEvents.push(tempEvent);
            }
        }
    }

    // below threshold?
    if (lastCCvalue < threshold)
    {
        // send off event if paused since enough time
        if (enabled && (pauseTimeInSamples >= inputParameters[4]*.001 * sampleRate))
        {
            enabled = false;
            tempEvent.timeStamp = 0;
            MidiEventUtils::setCCNumber(tempEvent, destCC);
            MidiEventUtils::setCCValue(tempEvent, 0);
            data.outputMidiEvents.push(tempEvent);
        }

        // update pause time
        pauseTimeInSamples += data.samplesToProcess;
    }
}