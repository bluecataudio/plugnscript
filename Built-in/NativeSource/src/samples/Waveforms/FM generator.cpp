#include "dspapi.h"
#include "cpphelpers.h"
#include <math.h>

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioOutputsCount=0;

/** \file
*   Freequency Modulation (FM) waveform generator.
*   Generates an audio waveform using frequency modulation on a sine wave.
*/
#include "../library/Constants.h"

DSP_EXPORT string name="FM Generator";
DSP_EXPORT string description="Frequency Modulation generator";

DSP_EXPORT array<string> inputParametersNames={"Amplitude", "Frequency","FM Index","Ratio"};
DSP_EXPORT array<string> inputParametersUnits={"%", "Hz","",""};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersMin={0,0,-1,0};
DSP_EXPORT array<double> inputParametersDefault={50,200,0};
DSP_EXPORT array<double> inputParametersMax={100,5000,-1,10};

double amplitude=0;
double omega=0;
double currentPhase=0;

double modulatorOmega=0;
double modulatorCurrentPhase=0;
double modulationIndex=0;

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
    double sampleValue=amplitude*sin(currentPhase+modulationIndex*sin(modulatorCurrentPhase));
        
    // update phase
    currentPhase+=omega;
    modulatorCurrentPhase+=modulatorOmega;
        
    // copy to all audio outputs
    for(uint channel=0;channel<audioOutputsCount;channel++)
    {
        ioSample[channel]=sampleValue;
    }
    
    // to avoid overflow, reduce phase
    while(currentPhase>period)
        currentPhase-=period;
    while(modulatorCurrentPhase>period)
        modulatorCurrentPhase-=period;

}

DSP_EXPORT void updateInputParameters()
{
   amplitude=inputParameters[0]*.01;
   omega=omegaFactor*inputParameters[1];
   modulationIndex=10*inputParameters[2];
   modulatorOmega=omega*inputParameters[3];
}

DSP_EXPORT int getTailSize()
{
    return -1;
}