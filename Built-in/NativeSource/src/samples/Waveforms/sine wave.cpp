#include "dspapi.h"
#include "cpphelpers.h"
#include <math.h>

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioOutputsCount=0;

/** \file
*   Sine wave generator.
*   Generates a sine wave using the sin() function.
*/

#include "../library/Constants.h"

DSP_EXPORT string name="Sine Wave";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Sine wave generator";

DSP_EXPORT array<string> inputParametersNames={"Amplitude", "Frequency"};
DSP_EXPORT array<string> inputParametersUnits={"%", "Hz"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersMin={0,0};
DSP_EXPORT array<double> inputParametersDefault={50,200};
DSP_EXPORT array<double> inputParametersMax={100,5000};

double amplitude=0;
double omega=0;
double currentPhase=0;
const double period=2*PI;
double omegaFactor=0;

DSP_EXPORT bool initialize()
{
    omegaFactor=period/sampleRate;
    return true;
}

DSP_EXPORT void processSample(double ioSample[])
{
    // compute sample value
    double sampleValue=amplitude*sin(currentPhase);
        
    // update phase
    currentPhase+=omega;
        
    // copy to all audio outputs
    for(uint channel=0;channel<audioOutputsCount;channel++)
    {
        ioSample[channel]=sampleValue;
    }
    
    // to avoid overflow, reduce phase
    while(currentPhase>period)
        currentPhase-=period;
}

DSP_EXPORT void updateInputParameters()
{
   amplitude=inputParameters[0]*.01;
   omega=omegaFactor*inputParameters[1];
}

DSP_EXPORT int getTailSize()
{
    return -1;
}