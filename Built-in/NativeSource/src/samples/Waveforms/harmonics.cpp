#include "dspapi.h"
#include "cpphelpers.h"
#include <math.h>

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioOutputsCount=0;

/** \file
*   Additive synthesis waveform generator.
*   Generates an audio waveform using additive synthesis with multiple
*   sine waves.
*/
#include "../library/Constants.h"

DSP_EXPORT string name="Harmonics";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Wave generator using overtones";

DSP_EXPORT array<string> inputParametersNames={"Amplitude", "Frequency","h1","h2","h3","h4","h5","h6","h7","Phase 1","Phase 2","Phase 3","Phase 4","Phase 5","Phase 6","Phase 7"};
DSP_EXPORT array<string> inputParametersUnits={"%", "Hz"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersMin={0,0};
DSP_EXPORT array<double> inputParametersDefault={50,200,1};
DSP_EXPORT array<double> inputParametersMax={100,5000};

double amplitude=0;
double omega=0;
double currentPhase=0;
double h0=0;
double h1=0;
double h2=0;
double h3=0;
double h4=0;
double h5=0;
double h6=0;
double ph0=0;
double ph1=0;
double ph2=0;
double ph3=0;
double ph4=0;
double ph5=0;
double ph6=0;

const double period=2*PI;
double omegaFactor=0;// to be initialized with sampleRate

DSP_EXPORT bool initialize()
{
    omegaFactor=period/sampleRate;
    return true;
}

DSP_EXPORT void processSample(double ioSample[])
{
    // compute sample value
    double sampleValue=amplitude*(h0*sin(currentPhase+ph0)
                                  +h1*sin(2*currentPhase+ph1)
                                  +h2*sin(3*currentPhase+ph2)
                                  +h3*sin(4*currentPhase+ph3)
                                  +h4*sin(5*currentPhase+ph4)
                                  +h5*sin(6*currentPhase+ph5)
                                  +h6*sin(7*currentPhase+ph6));
        
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
   h0=inputParameters[2];
   h1=inputParameters[3];
   h2=inputParameters[4];
   h3=inputParameters[5];
   h4=inputParameters[6];
   h5=inputParameters[7];
   h6=inputParameters[8];
   ph0=inputParameters[9]*period;
   ph1=inputParameters[10]*period;
   ph2=inputParameters[11]*period;
   ph3=inputParameters[12]*period;
   ph4=inputParameters[13]*period;
   ph5=inputParameters[14]*period;
   ph6=inputParameters[15]*period;
}

DSP_EXPORT int getTailSize()
{
    return -1;
}