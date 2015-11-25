// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioInputsCount=0;
DSP_EXPORT uint    audioOutputsCount=0;

// extra headers
#include <string>

/** \file
*   Multiple Channels Swapper.
*   Select source channel for each audio output.
*/

// metadata
DSP_EXPORT string name="Multi Channel Swapper";
DSP_EXPORT string description="Select source channel for each audio output";

// parameters definition: each parameter correponds to an audio output and lets you choose an input channel
DSP_EXPORT array<string>  inputParametersNames={};
DSP_EXPORT array<double>  inputParameters={};
DSP_EXPORT array<double>  inputParametersMin={};
DSP_EXPORT array<double>  inputParametersMax={};
DSP_EXPORT array<double>  inputParametersDefault={};
DSP_EXPORT array<int>     inputParametersSteps={};
DSP_EXPORT array<string>  inputParametersEnums={};

// arrays that actually contain the strings
array<std::string> stdInputParametersNames;
array<std::string> stdInputParametersEnums;

array<int>      sourceChannel;
array<double>   temp;

DSP_EXPORT bool initialize()
{
    // initialize our arrays
    inputParametersNames.resize(audioOutputsCount);
    stdInputParametersNames.resize(audioOutputsCount);
    inputParameters.resize(audioOutputsCount);
    inputParametersMin.resize(audioOutputsCount);
    inputParametersMax.resize(audioOutputsCount);
    inputParametersDefault.resize(audioOutputsCount);
    inputParametersSteps.resize(audioOutputsCount);
    inputParametersEnums.resize(audioOutputsCount);
    stdInputParametersEnums.resize(audioOutputsCount);
    
    sourceChannel.resize(audioInputsCount);
    temp.resize(audioInputsCount+1);
    temp[0]=0;
    
    // initialize parameters properties (depend on the number of i/o channels)
    for(uint i=0; i<audioOutputsCount;i++)
    {
        inputParametersMin[i]=0;
        inputParametersMax[i]=audioInputsCount;
        inputParametersSteps[i]=audioInputsCount+1;

        stdInputParametersNames[i]="Out "+std::to_string(i+1);
        inputParametersNames[i]=stdInputParametersNames[i].c_str(); // just points to std::string buffers
        if(i<=audioInputsCount)
            inputParametersDefault[i]=i+1;
        else
            inputParametersDefault[i]=0;
        stdInputParametersEnums[i]="None";
        for(uint ch=0;ch<audioInputsCount;ch++)
        {
            stdInputParametersEnums[i]+=";";
            stdInputParametersEnums[i]+="Channel "+std::to_string(ch+1);
            inputParametersEnums[i]=stdInputParametersEnums[i].c_str();
        }
    }
    return true;
}

DSP_EXPORT void processSample(double ioSample[])
{
    // copy inputs to temp buffer
    for(uint ch=0;ch<audioInputsCount;ch++)
    {
        temp[ch+1]=ioSample[ch];
    }
    
    // copy selected channels to outputs
    for(uint ch=0;ch<audioOutputsCount;ch++)
    {
        ioSample[ch]=temp[sourceChannel[ch]];
    }
}

DSP_EXPORT void updateInputParametersForBlock(const TransportInfo* info)
{
    // copy source channels selection
    for(uint channel=0;channel<audioOutputsCount;channel++)
    {
        sourceChannel[channel]=int(inputParameters[channel]+.5);
    }
}