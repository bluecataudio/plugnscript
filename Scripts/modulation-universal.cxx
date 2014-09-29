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

uint DELAY_WIDTH = 8192;

/** Define our parameters.
*/
array <string> inputParametersNames={"Min","Max","Rate","Feedback","Dry/Wet"};
array <string> inputParametersUnits={"ms","ms","%","%","%"};
array <double> inputParameters(inputParametersNames.length);
array <double> inputParametersDefault = { 7, 20,  30,    0,  50};
array <double> inputParametersMin     = { 0,  0,   0, -100,   0};
array <double> inputParametersMax     = {50, 50, 100,  100, 100};

string name = "Universal Modulation";
string author = "Ivan COHEN";
string description = "Algorithm to create chorus, vibrato and flanger effects";

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
    double phase;

    void reset(int output)
    {   v1 = yold = 0;
        phase = 1;
        if (output % 2 == 1) phase += PI/2;

        for(uint i=0; i<DELAY_WIDTH; i++)
            buffer[i] = 0;
    }

    void processSample(double& sample)
    {
        const double input=sample;

        phase += factor;
        if (phase > 2 * PI) phase -= 2 * PI;

        const double lfo = (maxd - mind) * (0.5 * sin(phase) + 0.5) + mind + 1;
        const double lfoF = b0 * lfo + v1;
        v1 = b1 * lfo - a1 * lfoF;

        const int k = int (floor(lfoF));
        const double frac = lfoF - k;

        const double y = buffer[(cpt + k) & (DELAY_WIDTH-1)] * (1 - frac) 
                       + buffer[(cpt + k + 1) & (DELAY_WIDTH-1)] - (1 - frac) * yold;
        yold = y;

        buffer[cpt] = input - y * feedback;

        // dry/wet
        sample = y * wet + input * dry;

        // denormalization
        if (! (v1 < -1.0e-8 || v1 > 1.0e-8)) v1 = 0;
        if (! (yold < -1.0e-8 || yold > 1.0e-8)) yold = 0;
    }

    
    
};

// Define our objects
array<sampleProcessor> processors(audioOutputsCount);


// Reset function
void reset()
{
    for(uint i=0;i<audioOutputsCount;i++)
    {
        processors[i].reset(i);
    }

    cpt = DELAY_WIDTH-1;
}

/** update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    mind = inputParameters[0]/1000*sampleRate;
    maxd = inputParameters[1]/1000*sampleRate;

    const double rate = pow(10, (inputParameters[2]/100-0.5)*1.3);
    factor = (2 * PI * rate / sampleRate);

    feedback = (inputParameters[3]/100) * 0.9;

    dry = 1 - (inputParameters[4]/100);
    wet = (inputParameters[4]/100);

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
	for(uint i=0;i<audioOutputsCount;i++)
	{
        processors[i].processSample(ioSample[i]);
    }

    cpt--; if (cpt < 0) cpt = DELAY_WIDTH-1;
}


