// =====================================================================================
// =====================================================================================
// 
// a Basic Multiband EQ by Margaux Morales.
// faderboy [at] faderboy.com
//
// An LF/LM/HM/HF four band equalizer
// Based on Ilya Orlov's Tutorial:
// https://pns.letimix.com/how-to/make-a-simple-low-cut-filter
//
// =====================================================================================
// =====================================================================================

/* include your dsp files.
*
*/
#include "library/BiquadFilter.hxx"
#include "library/Constants.hxx"

/* Define our parameters.
*   One parameter for cutoff frequency
*/
array<string> inputParametersNames={"LF Freq","LF Boost","LM Freq","LM Width","LM Boost","HM Freq","HM Width","HM Boost","HF Freq","HF Boost"};
array<double> inputParameters(inputParametersNames.length);
array<string> inputParametersUnits={"Hz","dB","Hz", "", "dB","Hz", "", "dB","Hz","dB"}; // which units to use
array<double> inputParametersDefault={160, 0, 5600, 0, 0, 9600, 0, 0, 8000, 0};
array<double> inputParametersMin={20, -18, 20, 0, -18, 20, 0, -18, 4800, -18}; // min value
array<double> inputParametersMax={560,18, 20000, 1, 18, 20000, 1, 18, 16000, 18}; // max value

string name="Multiband Equalizer";

string description="An LF/LM/HM/HF four band equalizer";

/* Define our internal variables.
// First We'll define four filters: LF Shelf,LM Peak, HM Peak, HF Shelf
*/
KittyDSP::Biquad::Filter lfFilter(audioInputsCount);
KittyDSP::Biquad::Filter hfFilter(audioInputsCount);
KittyDSP::Biquad::Filter lmFilter(audioInputsCount);
KittyDSP::Biquad::Filter hmFilter(audioInputsCount);

// Values for Peak Filters
double bwRange=1; // in half octaves
double minBw=1.0/10.0;

 
// index of input param just for convenience
// these make building GUIs much easier
const int LFFREQ = 0;
const int LFBOOST = 1;
const int LMFREQ= 2;
const int LMWIDTH = 3;
const int LMBOOST = 4;
const int HMFREQ= 5;
const int HMWIDTH = 6;
const int HMBOOST = 7;
const int HFFREQ = 8;
const int HFBOOST = 9;

/* per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{
    //Call each of our filters and process the ioSample
    lfFilter.processSample(ioSample);
    hfFilter.processSample(ioSample);
    lmFilter.processSample(ioSample);
    hmFilter.processSample(ioSample);
}

/* update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    //Read input values and setup each respective filter
    lfFilter.setLowShelf(2 * PI * inputParameters[LFFREQ] / sampleRate, sqrt(pow(10, inputParameters[LFBOOST] / 40.0)));
    hfFilter.setHighShelf(2 * PI * inputParameters[HFFREQ] / sampleRate, sqrt(pow(10, inputParameters[HFBOOST] / 40.0)));
    lmFilter.setPeak(2 * PI * inputParameters[LMFREQ] / sampleRate, minBw+inputParameters[LMWIDTH]*bwRange, sqrt(pow(10, inputParameters[LMBOOST] / 20.0)));
    hmFilter.setPeak(2 * PI * inputParameters[HMFREQ] / sampleRate, minBw+inputParameters[HMWIDTH]*bwRange, sqrt(pow(10, inputParameters[HMBOOST] / 20.0)));
}