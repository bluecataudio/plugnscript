// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount=0;

#include <string>

/** \file
*   Multichannel gain.
*   Apply different gain to individual channels.
*/
DSP_EXPORT string name="Multi Gain";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="multi channel gain";

DSP_EXPORT array<string> inputParametersNames={};
DSP_EXPORT array<double> inputParameters={};
array<double> gain={};

// arrays that actually contain the strings
array<std::string> stdInputParametersNames;

DSP_EXPORT bool initialize()
{
    // initialize our arrays
    inputParametersNames.resize(audioInputsCount);
    stdInputParametersNames.resize(audioInputsCount);
    inputParameters.resize(audioInputsCount);
    gain.resize(audioInputsCount);
    
    if(audioInputsCount==1)
        inputParametersNames[0]="Gain";
    else if(audioInputsCount==2)
    {
        inputParametersNames[0]="Left";
        inputParametersNames[1]="Right";
    }
    else
    {
        for(uint i=0;i<inputParametersNames.length;i++)
        {
            stdInputParametersNames[i]="Ch"+std::to_string(i+1);
            inputParametersNames[i]=stdInputParametersNames[i].c_str();
        }
    }
    return true;
}

DSP_EXPORT void processSample(double ioSample[])
{
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        ioSample[channel]*=gain[channel];
    }
}

DSP_EXPORT void updateInputParameters()
{
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        gain[channel]=inputParameters[channel];
    }
}