// =====================================================================================
// =====================================================================================
// Made by Ivan COHEN, for Blue Cat Audio Plug'n Script
//
// http://musicalentropy.wordpress.com/
// =====================================================================================
// =====================================================================================

/** include your dsp files.
*
*/
#include "./library/Constants.hxx"

/** Define our parameters.
*/
array <string> inputParametersNames={"Threshold","Ratio","Attack","Release","Makeup","Dry/Wet"};
array <string> inputParametersUnits={"dB","","ms","ms","dB","%"};
array <double> inputParameters(inputParametersNames.length);
array <double> inputParametersDefault = {  0,  1,   1,  40,  0, 100};
array <double> inputParametersMin     = {-60,  1,   1,  20,  0,   0};
array <double> inputParametersMax     = {  0, 20, 100, 500, 40, 100};

string name = "Standard Compressor";
string author = "Ivan COHEN";
string description = "Very simple RMS compression algorithm";

// Define our internal variables.
double threshold, thrinv, ratioinv;
double attack, release, rms;
double makeup;
double dry, wet;
double y1old, y2old;


// Reset function
void reset()
{
    y1old = y2old = 0;
}


/** update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    const double Te = 1.0 / sampleRate;
    
    threshold = pow(10, inputParameters[0] / 10);
    thrinv = 1 / threshold;
    ratioinv = 1.f / inputParameters[1];
    
    makeup = pow(10, inputParameters[4] / 20);
    
    attack  = exp(-2 * PI * Te * 1000 / inputParameters[2]);
    release = exp(-2 * PI * Te * 1000 / inputParameters[3]);    
    rms = exp(-2 * PI * Te * 1000 / 20);

    dry = 1 - (inputParameters[5]/100);
    wet = (inputParameters[5]/100);
}


/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{		
	double input = 0;
    for(uint i=0;i<audioOutputsCount;i++)
        input += ioSample[i];

    input = input / audioOutputsCount;

    const double y1 = rms * input * input + (1 - rms) * y1old;
    y1old = y1;
                
    const double cte = (y1 > y2old) ? attack : release;        
    const double y2 = (1 - cte) * y1 + cte * y2old;
    y2old = y2;
            
    const double attenuation = (y2 < threshold) ? 1.0 : pow(y2 * thrinv, 0.5*(ratioinv - 1));
            
    for(uint i=0;i<audioOutputsCount;i++)
        ioSample[i] *= dry + attenuation * makeup * wet;

    // denormalization
    if (! (y1old < -1.0e-8 || y1old > 1.0e-8)) y1old = 0;
    if (! (y2old < -1.0e-8 || y2old > 1.0e-8)) y2old = 0;

}
