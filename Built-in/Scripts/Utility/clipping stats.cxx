/** \file
*   Simple 0dB clipping statistics display.
*/

string description="Counts Max nb of consecutive samples at 0+ dB";
array<string> outputParametersNames(audioInputsCount);
array<double> outputParameters(audioInputsCount);
array<double> outputParametersMin(audioInputsCount);
array<double> outputParametersMax(audioInputsCount);

// statistics class for for each channel
class ChannelClipCount
{
    int currentClipsCount=0;
    int maxClipCount=0;
    int64 lastClipSample=0;

    void AddClippingValueForSample(int64 currentSample)
    {
        // check if last clipping sample was not the previous one => reset clips count
        if(currentSample-lastClipSample!=1)
            currentClipsCount=0;

        // increment current clips count
        currentClipsCount++;

        // store last clipping sample value
        lastClipSample=currentSample;

        // update max value
        if(currentClipsCount>maxClipCount)
            maxClipCount=currentClipsCount;
    }
};
array<ChannelClipCount> channelData(audioInputsCount);
int64                   currentSample=0;


void initialize()
{
    //initialize output parameters (uses audio inputs count)
    for(uint i=0;i<audioInputsCount;i++)
    {
        outputParametersNames[i]="Ch "+(i+1)+" Clips";
        outputParametersMin[i]=0;
        outputParametersMax[i]=128;
    } 
}

void processSample(array<double>& ioSample)
{
    // for each channel
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        // check if channel is "clipping"
        double value=abs(ioSample[channel]);
        if(value>=1)
        {
            channelData[channel].AddClippingValueForSample(currentSample);
        }
    }
    currentSample++;
}

void reset()
{
    // reset state
    for(uint i=0;i<channelData.length;i++)
    {
        channelData[i].lastClipSample=-1;
    }
    currentSample=0;
}

void computeOutputData()
{
    // transfer channels status to output parameters
    for(uint i=0;i<audioInputsCount;i++)
    {
        outputParameters[i]=channelData[i].maxClipCount;
    }
}