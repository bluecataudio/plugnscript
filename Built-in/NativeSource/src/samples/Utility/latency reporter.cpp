// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

/** \file
*   Fake Latency reporter.
*   To report a chosen latency to host application, to force delay compensation.
*/

DSP_EXPORT string name="Latency Reporter";
DSP_EXPORT string description="Reports chosen latency to host";

DSP_EXPORT array<string> inputParametersNames={"Latency"};
DSP_EXPORT array<string> inputParametersUnits={"Samp"};
DSP_EXPORT array<double> inputParametersMin={0};
DSP_EXPORT array<double> inputParametersMax={1024};
DSP_EXPORT array<double> inputParametersDefault={0};
DSP_EXPORT array<int>    inputParametersSteps={1025};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);

DSP_EXPORT int getLatency()
{
    return int(inputParameters[0]+.5);
}