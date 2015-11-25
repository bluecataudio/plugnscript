// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount=0;
DSP_EXPORT double  sampleRate=0;

#include <math.h>
#include <string>

/** \file
*   Multichannel peak levels meter.
*/

// Output parameters definition
DSP_EXPORT array<string> outputParametersNames={};
DSP_EXPORT array<string> outputParametersUnits={};
DSP_EXPORT array<double> outputParameters={};
DSP_EXPORT array<double> outputParametersMin={};
DSP_EXPORT array<double> outputParametersMax={};

// arrays that actually contain the strings
array<std::string> stdOutputParametersNames;

// meter decay
double decay=0;

// levels array
array<double>   levels={};

DSP_EXPORT bool initialize()
{
    // init arrays sizes
    outputParametersNames.resize(audioInputsCount);
    outputParametersUnits.resize(audioInputsCount);
    outputParameters.resize(audioInputsCount);
    outputParametersMin.resize(audioInputsCount);
    outputParametersMax.resize(audioInputsCount);
    stdOutputParametersNames.resize(audioInputsCount);
    levels.resize(audioInputsCount);
    
    // initialize decay variable
    decay=1-exp(-log(10)/(sampleRate*.3));
    
    // initialize output parameters properties
    if(audioInputsCount==1)
        outputParametersNames[0]="Level";
    else if(audioInputsCount==2)
    {
        outputParametersNames[0]="Left";
        outputParametersNames[1]="Right";
    }
    else
    {
        for(uint i=0;i<outputParametersNames.length;i++)
        {
            stdOutputParametersNames[i]="Level Ch"+std::to_string(i+1);
            outputParametersNames[i]=stdOutputParametersNames[i].c_str();
        }
    }
    for(uint i=0;i<audioInputsCount;i++)
    {
        outputParametersUnits[i]="dB";
        outputParametersMin[i]=-60;
        outputParametersMax[i]=0;
    }
    
    return true;
}

/* per-sample processing.
*
*/
DSP_EXPORT void processSample(double ioSample[])
{
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        double value=fabs(ioSample[channel]);
        double level=levels[channel];
        if(value>level)
            level=value;
        else
            level+=decay*(value-level);
        levels[channel]=level;
    }
}

DSP_EXPORT int getTailSize()
{
    // 1000 milliseconds to let meters reach -60 dB
    return int(1*sampleRate);
}

DSP_EXPORT void reset()
{
    // reset levels to 0
    for(uint i=0;i<levels.length;i++)
    {
        levels[i]=0;
    }
}

DSP_EXPORT void computeOutputData()
{
    for(uint i=0;i<audioInputsCount;i++)
    {
        if(levels[i]>pow(10,-3)) // -60 dB limit
            outputParameters[i]=20*log10(levels[i]);
        else
            outputParameters[i]=-60;
    }
}