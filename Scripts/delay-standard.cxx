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

uint DELAY_WIDTH = 131072;

/** Define our parameters.
*/
array <string> inputParametersNames={"Delay","Feedback","Cutoff","Dry/Wet"};
array <string> inputParametersUnits={"ms","%","%","%"};
array <double> inputParameters(inputParametersNames.length);
array <double> inputParametersDefault={200, 10, 50, 50};
array <double> inputParametersMin={0, 0, 0, 0};
array <double> inputParametersMax={1000, 100, 100, 100};

string name = "Standard Delay";
string author = "Ivan COHEN";
string description = "Delay algorithm with feedback and lowpass filtering";

// Define our internal variables.
double a1, b0, b1;
double a1d, b0d, b1d;
double dry, wet;
double feedback;
double delay;
int cpt;


// Classes definition
class sampleProcessor
{
    array <double> buffer(DELAY_WIDTH);
    double v1, v1d;

    void processSample(double& sample)
    {
        const double input=sample;

        double delayF = b0d*delay + v1d;
        v1d = b1d*delay - a1d*delayF;

        uint delayI = uint(floor(delayF))+1;

        double y = buffer[(cpt + delayI) & (DELAY_WIDTH-1)];
        
        double yF = b0 * y + v1;
        v1 = b1*y - a1*yF;

        buffer[cpt] = input - yF*feedback;
        
        sample = dry*input + wet*yF;
        
        // denormalization
        if (! (v1 < -1.0e-8 || v1 > 1.0e-8)) v1 = 0;
        if (! (v1d < -1.0e-8 || v1d > 1.0e-8)) v1d = 0;
    }

    void reset()
    {  	v1 = v1d = 0;
        for(uint i=0; i<DELAY_WIDTH; i++)
            buffer[i] = 0;
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
    cpt = DELAY_WIDTH-1;
}

/** update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    delay = inputParameters[0]/1000*sampleRate;
    feedback = inputParameters[1]/100;

    double cutoff = pow(10, inputParameters[2]/100*(log10(20000)-log10(40))+log10(40));
    double tanw0 = tan(PI * cutoff/sampleRate);
    double tanw0plusinv = 1.0 / (tanw0 + 1.0); 

    a1 = (tanw0 - 1.0) * tanw0plusinv;
    b0 = tanw0 * tanw0plusinv;
    b1 = b0;

    double tanw0d = tan(PI * 40/sampleRate);
    double tanw0dplusinv = 1.0 / (tanw0d + 1.0); 

    a1d = (tanw0d - 1.0) * tanw0dplusinv;
    b0d = tanw0d * tanw0dplusinv;
    b1d = b0d;
    
    dry = 1 - (inputParameters[3]/100);
    wet = (inputParameters[3]/100);
}

/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processBlock(BlockData& data)
{   
    const int cpt_cours = cpt;

    for(uint ch=0;ch<audioOutputsCount;ch++)
    {
        array<double>@ samples=data.samples[ch];
        sampleProcessor@ processor=processors[ch];
        cpt = cpt_cours;

        for(uint i=0; i<data.samplesToProcess; i++)
        {
           processor.processSample(samples[i]);
           cpt--; if (cpt < 0) cpt = DELAY_WIDTH-1;
        }
    }
    
}


