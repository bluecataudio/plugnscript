/** \file
*   Notch filter.
*/

/* include your dsp files.
*
*/
#include "../library/BiquadFilter.hxx"
#include "../library/Constants.hxx"

/* Define our parameters.
*   One parameter for cutoff frequency
*/
array<string> inputParametersNames={"Frequency","Bandwidth"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersDefault={.5,0};

/* Define our internal variables.
*
*/
KittyDSP::Biquad::Filter filter(audioInputsCount);
double freqFactor=2*PI*20/sampleRate;
double bwRange=1; // in half octaves
double minBw=1.0/10.0;

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
    filter.setNotch(freqFactor*pow(1000,inputParameters[0]),minBw+inputParameters[1]*bwRange);
}
