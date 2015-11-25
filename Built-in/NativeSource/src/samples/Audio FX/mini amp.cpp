// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioInputsCount=0;

// extra system headers
#include <math.h>

/** \file
*   Mini guitar amp simulator with distortion.
*   Produces hard-clipping (transistor-like) distortion that can be attenuated with
*   a low pass filter, that simulates the cabinet.
*/

/* include our dsp classes.
*
*/
#include "../library/BiquadFilter.h"
#include "../library/Constants.h"

/* Script metadata.
*/
DSP_EXPORT string name="Mini Amp";
DSP_EXPORT string description="mini guitar amp";

/* Define our parameters.
*/
DSP_EXPORT array<string> inputParametersNames={"Gain","Filter","Post Gain"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersDefault={.4,.2,.5};

/* Define our internal variables.
*
*/
KittyDSP::Biquad::Filter filter;
double freqRange=0;
double gain=0;
double postGain=1;


DSP_EXPORT bool initialize()
{
    freqRange=2*PI*1000/sampleRate;
    filter.setChannelsCount(audioInputsCount);
    return true;
}

/* per-sample processing function: called for every sample with updated parameters values.
*
*/
DSP_EXPORT void processSample(double ioSample[])
{  
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        // apply gain 
        double sample=ioSample[channel];
        sample*=gain;

        // simple hard clipping
        if(sample>1)
            sample=1;
        else if(sample<-1)
            sample=-1;

        // reduce gain (post filtering may introduce clipping otherwise)
        sample*=postGain;

        // low pass filter
        filter.processSample(sample,channel);

        // copy back
        ioSample[channel]=sample;
    }
}

/* update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
DSP_EXPORT void updateInputParameters()
{
    // input gain (1 to 100)
    gain=pow(10,inputParameters[0]*10);

    // set lowpass filter frequency
    double freq=freqRange*pow(10,1.3*(1-inputParameters[1]));
    if(freq>1)
        freq=1;
    filter.setLowPass(freq);
    // post gain: from .1 to 1
    postGain=pow(10,-1+inputParameters[2]);
}
