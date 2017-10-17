// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioInputsCount=0;

// extra system headers
#include <math.h>

/** \file
*   Ring modulator effect.
*   Classic ring modulator effect that multiplies the signal with a sine wave.
*/

#include "../library/Constants.h"

DSP_EXPORT string name="Ring Mod";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="ring modulator";

/* Define our parameters.
*/
DSP_EXPORT array<string> inputParametersNames={"Frequency","Mix"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersDefault={.5,.5};

// internal variables
int index=0;
double omega=.01;
int period=0;

DSP_EXPORT void reset()
{
    index=0;
}

/* per-sample processing function: called for every sample with updated parameters values.
*
*/
DSP_EXPORT void processSample(double ioSample[])
{
    // compute value once
    double coeff=sin(omega*index);
    index++;
    index%=period;
    coeff=(1+(coeff-1)*inputParameters[1]); // dry/wet

    // multiply all channels
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        ioSample[channel]*=coeff;
    }
}

DSP_EXPORT void updateInputParameters()
{
    omega=.001+inputParameters[0];
    period=int(2*PI/omega+.5);
}
