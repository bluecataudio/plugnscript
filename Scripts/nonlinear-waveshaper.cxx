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
array <string> inputParametersNames={"Pre-HP","Post-LP","Drive","Output","Dry/Wet"};
array <string> inputParametersUnits={"%","%","dB","dB","%","%"};
array <double> inputParameters(inputParametersNames.length);
array <double> inputParametersDefault = {0,   100,   0,   0, 100};
array <double> inputParametersMin     = {0,     0,   0, -40,   0};
array <double> inputParametersMax     = {100, 100,  40,  40, 100};

string name = "Waveshaper";
string author = "Ivan COHEN";
string description = "Waveshaper with pre HP and post LP filters";

// Define our internal variables.
double a1L, b0L, b1L;
double a1H, b0H, b1H;
double gain, volume;
double dry, wet;


// Classes definition
class sampleProcessor
{
    double v1L, v1H;

    void reset()
    {  	v1L = v1H = 0;
    }

    void processSample(double& sample)
    {
        const double input=sample;

        // Pre High-Pass filter
        const double yH = b0H * input + v1H;
        v1H = b1H * input - a1H * yH;

        // Waveshaping function
        const double yNL = tanh(gain*(yH))*0.9;

        // Post Low-Pass filter
        const double yL = b0L * yNL + v1L;
        v1L = b1L * yNL - a1L * yL;

        // Final result
        sample = (dry*input + wet*yL) * volume;
        
        // denormalization
        if (! (v1L < -1.0e-8 || v1L > 1.0e-8)) v1L = 0;
        if (! (v1H < -1.0e-8 || v1H > 1.0e-8)) v1H = 0;
    }
    
};

// Define our objects
array<sampleProcessor> processors(audioOutputsCount);


// Reset function
void reset()
{
    for(uint i=0;i<audioOutputsCount;i++)
    {
        processors[i].reset();
    }
}

/** update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    // Pre High-Pass filter
    const double cutoffH = pow(10, inputParameters[0]/100*(log10(20000)-log10(20))+log10(20));
    const double tanw0H = tan(PI * cutoffH / sampleRate);
	const double tanw0Hplusinv = 1.0 / (tanw0H + 1.0);	

	a1H = (tanw0H - 1.0) * tanw0Hplusinv;
	b0H = tanw0Hplusinv;
	b1H = -b0H;

    // Post Low-Pass filter
    const double cutoffL = pow(10, inputParameters[1]/100*(log10(20000)-log10(40))+log10(40));
    const double tanw0L = tan(PI * cutoffL / sampleRate);
    const double tanw0Lplusinv = 1.0 / (tanw0L + 1.0); 

    a1L = (tanw0L - 1.0) * tanw0Lplusinv;
    b0L = tanw0L * tanw0Lplusinv;
    b1L = b0L;
    
    // Other parameters
    gain = pow(10, (inputParameters[2])/20);
    volume = pow(10, (inputParameters[3])/20);
    dry = 1 - (inputParameters[4]/100);
    wet = (inputParameters[4]/100);
}

/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{		
	for(uint i=0;i<audioOutputsCount;i++)
	{
        processors[i].processSample(ioSample[i]);
    }
}


