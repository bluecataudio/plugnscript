// =====================================================================================
// =====================================================================================
// Made by Ivan COHEN, for Blue Cat Audio Plug'n Script
// Note : I didn't model the bass / treble sections for simplification purposes
//
// http://musicalentropy.wordpress.com/
// =====================================================================================
// =====================================================================================

/** include your dsp files.
*
*/
#include "./library/Constants.hxx"

uint DELAY_WIDTH = 4096;

/** Define our parameters.
*/
array <string> inputParametersNames={"Slow Freq","Fast Freq","Mode","Dry/Wet"};
array <string> inputParametersUnits={"Hz","Hz","","%"};
array <double> inputParameters(inputParametersNames.length);
array <double> inputParametersDefault = { 0.4,  6,  0, 100};
array <double> inputParametersMin     = {0.25,  5,  0,   0};
array <double> inputParametersMax     = {   2,  9,  1, 100};
array <int>    inputParametersSteps   = {  -1, -1,  2,  -1};
array <string> inputParametersEnums   = {  "", "", "slow;fast", ""};

string name = "Simple Leslie";
string author = "Ivan COHEN";
string description = "Very simple algorithm to emulate a Leslie rotating speaker";

// Define our internal variables.
double mind, maxd;
double feedback;
double dry, wet;
int cpt;
double factor;

double b0, b1, a1;

// Classes definition
class sampleProcessor
{
    array <double> buffer(DELAY_WIDTH);
    double v1, yold;
    double v1_inv, yold_inv;
    double phase;

    void reset()
    {   v1 = yold = 0;
        v1_inv = yold_inv = 0;

        phase = 1;

        for(uint i=0; i<DELAY_WIDTH; i++)
            buffer[i] = 0;
    }

    void processSample(double& sampleL, double& sampleR)
    {
        const double input = 0.5 * (sampleL + sampleR);

        phase += factor;
        if (phase > 2 * PI) phase -= 2 * PI;
        const double lfo = sin(phase);
        
        const double lfoD = (maxd - mind) * (0.5 * lfo + 0.5) + mind + 1;
        
        const int k = int (floor(lfoD));
        const double frac = lfoD - k;

        const double y = buffer[(cpt + k) & (DELAY_WIDTH-1)] * (1 - frac) 
                       + buffer[(cpt + k + 1) & (DELAY_WIDTH-1)] - (1 - frac) * yold;
        yold = y;

        const double lfoD_inv = (maxd - mind) * (0.5 - 0.5 * lfo) + mind + 1;

        const int k_inv = int (floor(lfoD_inv));
        const double frac_inv = lfoD_inv - k_inv;

        const double y_inv = buffer[(cpt + k_inv) & (DELAY_WIDTH-1)] * (1 - frac_inv) 
                           + buffer[(cpt + k_inv + 1) & (DELAY_WIDTH-1)] - (1 - frac_inv) * yold_inv;
        yold_inv = y_inv;

        buffer[cpt] = input;

        // tremolo
        const double output = y * (1 + lfo);
        const double output_inv = y_inv * (1 - lfo);

        // dry/wet
        sampleL = sampleL * dry + 0.5 * (output + 0.7 * output_inv) * wet;
        sampleR = sampleR * dry + 0.5 * (output_inv + 0.7 * output) * wet;

        // denormalization
        if (! (v1 < -1.0e-8 || v1 > 1.0e-8)) v1 = 0;
        if (! (yold < -1.0e-8 || yold > 1.0e-8)) yold = 0;

        if (! (v1_inv < -1.0e-8 || v1_inv > 1.0e-8)) v1_inv = 0;
        if (! (yold_inv < -1.0e-8 || yold_inv > 1.0e-8)) yold_inv = 0;
    }

    
    
};

// Define our objects
sampleProcessor processors;


// Reset function
void reset()
{
    processors.reset();
    cpt = DELAY_WIDTH-1;
}

/** update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    mind = 7.0/1000*sampleRate;
    maxd = mind + 0.5/1000*sampleRate;

    const double rate = (inputParameters[2] > 0.5) ? inputParameters[1] : inputParameters[0];
    factor = (2 * PI * rate / sampleRate);

    dry = 1 - (inputParameters[3]/100);
    wet = (inputParameters[3]/100);

    const double tanw0 = tan(40 * PI / sampleRate);
    const double tanw0plusinv = 1.0 / (tanw0 + 1.0); 

    a1 = (tanw0 - 1) * tanw0plusinv;
    b0 = tanw0 * tanw0plusinv;
    b1 = b0;
}

/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{		
	if (audioOutputsCount >= 2)
    {
        processors.processSample(ioSample[0], ioSample[1]);
        cpt--; if (cpt < 0) cpt = DELAY_WIDTH-1;
    }

    for(uint i=2;i<audioOutputsCount;i++)
	{
        ioSample[i] = 0;
    }
}


