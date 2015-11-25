// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioInputsCount=0;

// extra system headers
#include <math.h>

/** \file
*   Tremolo effect.
*   Classic tremolo effect (amplitude modulation).
*/

#include "../library/Constants.h"

DSP_EXPORT string name="Tremolo";
DSP_EXPORT string description="tremolo effect";

/* Define our parameters.
*/
DSP_EXPORT array<string>  inputParametersNames={"Rate","Mix"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersDefault={.5,.5,.6,.5};

// internal variables
double currentPhase=0;
double omega=0;
double mix=0;

// constants
double maxOmega=0; // initialized in initialize because depends on sampleRate
const double period=2*PI;

DSP_EXPORT bool initialize()
{
    maxOmega=10*2*PI/sampleRate; // up to 10 hz
    return true;
}

/* per-sample processing function: called for every sample.
*
*/
DSP_EXPORT void processSample(double ioSample[])
{
    // compute amplitude value once
    double coeff=.5*(1+sin(currentPhase));
    coeff=(1+(coeff-1)*mix); // apply dry-wet

    // multiply all channels
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        ioSample[channel]*=coeff;
    }
    
    // update phase
    currentPhase+=omega;
    if(currentPhase>period)
        currentPhase-=period;
}

DSP_EXPORT void updateInputParameters()
{
    omega=maxOmega*inputParameters[0];
    mix=inputParameters[1];
}

DSP_EXPORT void reset()
{
    currentPhase=0;
}
