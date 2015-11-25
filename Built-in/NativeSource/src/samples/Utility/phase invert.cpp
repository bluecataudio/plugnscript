// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount=0;

#include <string>

/** \file
*   Phaser inverter.
*   Invert the phase of the audio signal.
*/

/* Define our parameters.
*/
DSP_EXPORT array<string> inputParametersNames={"Invert"};
DSP_EXPORT array<string> inputParametersEnums={"Off;On"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<int> inputParametersSteps={2};
DSP_EXPORT array<double> inputParametersMin={0};
DSP_EXPORT array<double> inputParametersMax={1};

DSP_EXPORT string description="signal phase inverter";
double norm=1;

/* per-sample processing function: called for every sample with updated parameters values.
*
*/
DSP_EXPORT void processSample(double ioSample[])
{
    for(uint i=0;i<audioInputsCount;i++)
    {
        ioSample[i]*=norm;
    }
}

DSP_EXPORT void updateInputParameters()
{
    norm=-2*inputParameters[0]+1;
}