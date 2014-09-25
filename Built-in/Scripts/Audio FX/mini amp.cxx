/** include our dsp classes.
*
*/
#include "../library/BiquadFilter.hxx"
#include "../library/Constants.hxx"

/** Script metadata.
*/
string name="Mini Amp";
string description="mini guitar amp";

/** Define our parameters.
*/
array<string> inputParametersNames={"Gain","Filter","Post Gain"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersDefault={.4,.2,.5};
/** Define our internal variables.
*
*/
KittyDSP::Biquad::Filter filter(audioInputsCount);
double freqRange=2*PI*1000/sampleRate;
double gain=0;
double postGain=1;

/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
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

/** update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
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
