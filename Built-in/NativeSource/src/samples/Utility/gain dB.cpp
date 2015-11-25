// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"
#include "math.h"

DSP_EXPORT uint    audioInputsCount=0;

/** \file
*   Apply selected gain (decibels) to audio input.
*/
DSP_EXPORT string description="gain (dB)";

/* Define our input parameters.
*/
DSP_EXPORT array<string> inputParametersNames={"Gain"};
DSP_EXPORT array<string> inputParametersUnits={"dB"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersMin={-20};
DSP_EXPORT array<double> inputParametersMax={20};
DSP_EXPORT array<double> inputParametersDefault={0};

/* Define our internal variables.
*
*/
double gain=0;

/* per-sample processing function: called for every sample with updated parameters values.
*
*/
DSP_EXPORT void processSample(double ioSample[])
{
   for(uint channel=0;channel<audioInputsCount;channel++)
   {
      ioSample[channel]*=gain;
   }
}

/* update internal parameters from inputParameters array.
*   called every sample before processSample method
*/
DSP_EXPORT void updateInputParameters()
{
   /*using reverse dB formula: gain=10^(gaindB/20)*/
   gain=pow(10,inputParameters[0]/20);
}