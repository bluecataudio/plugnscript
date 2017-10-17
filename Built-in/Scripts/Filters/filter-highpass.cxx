/** \file
*   Highpass filter with resonance control.
*/

/* include your dsp files.
*
*/
#include "../library/BiquadFilter.hxx"
#include "../library/Constants.hxx"

/* Define our parameters.
*   One parameter for cutoff frequency
*/
array<string> inputParametersNames={"Frequency","Resonance"};
array<double> inputParameters(inputParametersNames.length);
string name="HP Filter";

/* Define our internal variables.
*
*/
KittyDSP::Biquad::Filter filter(audioInputsCount);
double freqFactor=2*PI*20/sampleRate;

/* per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{
    filter.processSample(ioSample);
}

/* update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    // -3dB boost (flat LP) to 20 dB boost 
    double q=pow(10,(-3+inputParameters[1]*23)/20);
    filter.setResonantHighPass(freqFactor*pow(1000,inputParameters[0]),q);
}