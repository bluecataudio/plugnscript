// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioInputsCount=0;

// extra system headers
#include <math.h>

/** \file
*   Highpass filter with resonance control.
*/

/* include your dsp files.
*
*/
#include "../library/BiquadFilter.h"
#include "../library/Constants.h"

/* Define our parameters.
*   One parameter for cutoff frequency
*/
DSP_EXPORT array<string> inputParametersNames={"Frequency","Resonance"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);

/* Define our internal variables.
*
*/
KittyDSP::Biquad::Filter filter;
double freqFactor=0;

DSP_EXPORT bool initialize()
{
    freqFactor=2*PI*20/sampleRate;
    filter.setChannelsCount(audioInputsCount);
    return true;
}

/* per-sample processing function: called for every sample with updated parameters values.
*
*/
DSP_EXPORT void processSample(double ioSample[])
{
    filter.processSample(ioSample);
}

/* update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
DSP_EXPORT void updateInputParameters()
{
    // -3dB boost (flat LP) to 20 dB boost 
    double q=pow(10,(-3+inputParameters[1]*23)/20);
    filter.setResonantHighPass(freqFactor*pow(1000,inputParameters[0]),q);
}