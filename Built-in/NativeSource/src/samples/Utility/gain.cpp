// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount=0;

/** \file
*   Apply selected gain to audio input.
*/

DSP_EXPORT string description="simple volume control";

DSP_EXPORT array<string> inputParametersNames={"Gain"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);

double gain=0;

DSP_EXPORT void processSample(double ioSample[])
{
   for(uint channel=0;channel<audioInputsCount;channel++)
   {
      ioSample[channel]*=gain;
   }
}

DSP_EXPORT void updateInputParameters()
{
   gain=inputParameters[0];
}