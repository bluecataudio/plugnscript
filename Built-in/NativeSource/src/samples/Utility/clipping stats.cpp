// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount=0;

#include <string>
#include <math.h>

/** \file
*   Simple 0dB clipping statistics display.
*/

DSP_EXPORT string description="Counts Max nb of consecutive samples at 0+ dB";
DSP_EXPORT array<string> outputParametersNames={};
DSP_EXPORT array<double> outputParameters={};
DSP_EXPORT array<double> outputParametersMin={};
DSP_EXPORT array<double> outputParametersMax={};

// arrays that actually contain the strings
array<std::string> stdOutputParametersNames;

// statistics class for for each channel
struct ChannelClipCount
{
    int     currentClipsCount=0;
    int     maxClipCount=0;
    int64   lastClipSample=0;

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

array<ChannelClipCount> channelData={};
int64                   currentSample=0;


DSP_EXPORT bool initialize()
{
    // init arrays sizes
    channelData.resize(audioInputsCount);
    outputParametersNames.resize(audioInputsCount);
    outputParameters.resize(audioInputsCount);
    outputParametersMin.resize(audioInputsCount);
    outputParametersMax.resize(audioInputsCount);
    stdOutputParametersNames.resize(audioInputsCount);
    
    //initialize output parameters (uses audio inputs count)
    for(uint i=0;i<audioInputsCount;i++)
    {
        stdOutputParametersNames[i]="Ch "+std::to_string(i+1)+" Clips";
        outputParametersNames[i]=stdOutputParametersNames[i].c_str();
        outputParametersMin[i]=0;
        outputParametersMax[i]=128;
    }
    return true;
}

DSP_EXPORT void processSample(double ioSample[])
{
    // for each channel
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        // check if channel is "clipping"
        double value=fabs(ioSample[channel]);
        if(value>=1)
        {
            channelData[channel].AddClippingValueForSample(currentSample);
        }
    }
    currentSample++;
}

DSP_EXPORT void reset()
{
    // reset state
    for(uint i=0;i<channelData.length;i++)
    {
        channelData[i].lastClipSample=-1;
    }
    currentSample=0;
}

DSP_EXPORT void computeOutputData()
{
    // transfer channels status to output parameters
    for(uint i=0;i<audioInputsCount;i++)
    {
        outputParameters[i]=channelData[i].maxClipCount;
    }
}