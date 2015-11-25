#include "dspapi.h"
#include "cpphelpers.h"
#include <math.h>

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioOutputsCount=0;

#include "../library/rand.h"

/** \file
*   Pseudo white noise generator.
*   Generates pseudo white noise using the rand() function.
*/

DSP_EXPORT string name="Pseudo White Noise";
DSP_EXPORT string description="simple pseudo white noise generator";

DSP_EXPORT array<string> inputParametersNames={"Amplitude"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersDefault={.5};

double amplitude=0;

DSP_EXPORT void processSample(double ioSample[])
{
   for(uint channel=0;channel<audioOutputsCount;channel++)
   {
      ioSample[channel]=rand(-amplitude,amplitude);
   }
}

DSP_EXPORT void updateInputParameters()
{
   amplitude=inputParameters[0];
}

DSP_EXPORT int getTailSize()
{
    return -1;
}