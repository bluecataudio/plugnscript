/** \file
 *   Monitor side chain or main input.
 */

 // C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount = 0;
DSP_EXPORT uint    auxAudioInputsCount = 0;

DSP_EXPORT string name="Side Chain A/B";
DSP_EXPORT string description="A/B with Side Chain Signal";

DSP_EXPORT array<string> inputParametersNames={"Source"};
DSP_EXPORT array<double> inputParametersMin={0};
DSP_EXPORT array<double> inputParametersMax={1};
DSP_EXPORT array<int>    inputParametersSteps={2};
DSP_EXPORT array<string> inputParametersEnums={"Main;Side Chain"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);

double inputGain=0;
double sideChainGain=0;
uint  commonInputsCount=0;

DSP_EXPORT bool initialize()
{
    // compute common number of channels
    commonInputsCount=audioInputsCount;
    if(auxAudioInputsCount<commonInputsCount)
        commonInputsCount=auxAudioInputsCount;
    return true;
}

DSP_EXPORT void processSample(double ioSample[])
{
    for(uint channel=0;channel<commonInputsCount;channel++)
    {
        ioSample[channel]=ioSample[channel]*inputGain+ioSample[channel+audioInputsCount]*sideChainGain;
    }
    for(uint channel=commonInputsCount;channel<audioInputsCount;channel++)
    {
        ioSample[channel]*=inputGain;
    }
}

DSP_EXPORT void updateInputParameters()
{
    sideChainGain=inputParameters[0];
    inputGain=1-sideChainGain;
}