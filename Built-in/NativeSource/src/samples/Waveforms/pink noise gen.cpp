#include "dspapi.h"
#include "cpphelpers.h"
#include <math.h>

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioOutputsCount=0;

#include "../library/rand.h"

/** \file
*   Pink noise generator.
*   Generates pseudo pink noise from white noise, using a fixed 3dB/Octave filter.
*/

DSP_EXPORT string name="Pseudo Pink Noise";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="simple pseudo pink noise generator";

DSP_EXPORT array<string> inputParametersNames={"Amplitude"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersDefault={.5};

double amplitude=0;
double b0=0;
double b1=0;
double b2=0;
double b3=0;
double b4=0;
double b5=0;
double b6=0;

DSP_EXPORT void processSample(double ioSample[])
{
    double white=rand(-amplitude,amplitude);
    b0 = 0.99886 * b0 + white * 0.0555179;
    b1 = 0.99332 * b1 + white * 0.0750759;
    b2 = 0.96900 * b2 + white * 0.1538520;
    b3 = 0.86650 * b3 + white * 0.3104856;
    b4 = 0.55000 * b4 + white * 0.5329522;
    b5 = -0.7616 * b5 - white * 0.0168980;
    double pink = b0 + b1 + b2 + b3 + b4 + b5 + b6 + white * 0.5362;
    b6 = white * 0.115926;
   for(uint channel=0;channel<audioOutputsCount;channel++)
   {
      ioSample[channel]=pink;
   }
}

DSP_EXPORT void updateInputParameters()
{
    // amplitude should be kept below .13 to avoid clipping, because of filtering
   amplitude=inputParameters[0]*.12;
}

DSP_EXPORT int getTailSize()
{
    return -1;
}