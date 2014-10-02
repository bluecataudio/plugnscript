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
array <string> inputParametersNames={"Delay 1","Delay 2","Delay 3","Delay 4","Feedback","Cutoff","Dry/Wet"};
array <string> inputParametersUnits={"","","","","%","%","%"};
array <double> inputParameters(inputParametersNames.length);
array <double> inputParametersDefault = { 9, 0, 0, 0,  10,  50,  50};
array <double> inputParametersMin     = { 0, 0, 0, 0,   0,   0,   0};
array <double> inputParametersMax     = {10,10,10,10, 100, 100, 100};
array <int>    inputParametersSteps   = {11,11,11,11,  -1,  -1,  -1};
array <string> inputParametersEnums   = {"OFF;1/64;1/32;1/24;1/16;1/8;1/6;1/4;1/3;1/2;1", 
                                         "OFF;1/64;1/32;1/24;1/16;1/8;1/6;1/4;1/3;1/2;1",
                                         "OFF;1/64;1/32;1/24;1/16;1/8;1/6;1/4;1/3;1/2;1",
                                         "OFF;1/64;1/32;1/24;1/16;1/8;1/6;1/4;1/3;1/2;1",
                                          "", "", ""};

string name = "Multitap Delay Sync";
string author = "Ivan COHEN";
string description = "Multitap delay algorithm with delay values sync with the host";

// Define our internal variables.
double a1, b0, b1;
double a1d, b0d, b1d;
double dry, wet;
double feedback;
double delay1, delay2, delay3, delay4;
int cpt;


// Classes definition
class sampleProcessor
{
    array <double> buffer(DELAY_WIDTH);
    double v1, v1d1, v1d2, v1d3, v1d4;

    void processSample(double& sample)
    {
        const double input=sample;

        const double delayF1 = b0d*delay1 + v1d1;
        v1d1 = b1d*delay1 - a1d*delayF1;
        uint delayI1 = uint(floor(delayF1))+1;

        const double delayF2 = b0d*delay2 + v1d2;
        v1d2 = b1d*delay2 - a1d*delayF2;
        uint delayI2 = uint(floor(delayF2))+1;
        
        const double delayF3 = b0d*delay3 + v1d3;
        v1d3 = b1d*delay3 - a1d*delayF3;
        uint delayI3 = uint(floor(delayF3))+1;
        
        const double delayF4 = b0d*delay4 + v1d4;
        v1d4 = b1d*delay4 - a1d*delayF4;
        uint delayI4 = uint(floor(delayF4))+1;
        
        double y = 0;

        if (delay1 > 0) y += buffer[(cpt + delayI1) & (DELAY_WIDTH-1)];
        if (delay2 > 0) y += buffer[(cpt + delayI2) & (DELAY_WIDTH-1)];
        if (delay3 > 0) y += buffer[(cpt + delayI3) & (DELAY_WIDTH-1)];
        if (delay4 > 0) y += buffer[(cpt + delayI4) & (DELAY_WIDTH-1)];

        y *= 0.25;
        
        double yF = b0 * y + v1;
        v1 = b1*y - a1*yF;

        buffer[cpt] = input - yF*feedback;
        
        sample = dry*input + wet*yF;
        
        // denormalization
        if (! (v1 < -1.0e-8 || v1 > 1.0e-8)) v1 = 0;
        if (! (v1d1 < -1.0e-8 || v1d1 > 1.0e-8)) v1d1 = 0;
        if (! (v1d2 < -1.0e-8 || v1d2 > 1.0e-8)) v1d2 = 0;
        if (! (v1d3 < -1.0e-8 || v1d3 > 1.0e-8)) v1d3 = 0;
        if (! (v1d4 < -1.0e-8 || v1d4 > 1.0e-8)) v1d4 = 0;
    }

    void reset()
    {   v1 = v1d1 = v1d2 = v1d3 = v1d4 = 0;
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
void updateInputParametersForBlock(const TransportInfo@ info)
{
    double bpm = 120;

    if (@info != null)
        bpm = info.bpm;
    
    double value;
    
    if (inputParameters[0] == 0) value = -1;
    else if (inputParameters[0] <= 1) value = 64;
    else if (inputParameters[0] <= 2) value = 32;
    else if (inputParameters[0] <= 3) value = 24;
    else if (inputParameters[0] <= 4) value = 16;
    else if (inputParameters[0] <= 5) value = 8;
    else if (inputParameters[0] <= 6) value = 6;
    else if (inputParameters[0] <= 7) value = 4;
    else if (inputParameters[0] <= 8) value = 3;
    else if (inputParameters[0] <= 9) value = 2;
    else value = 1;

    if (value == -1) delay1 = 0;
    else delay1 = sampleRate / bpm * 60 * 4 / value;

    if (inputParameters[1] == 0) value = -1;
    else if (inputParameters[1] <= 1) value = 64;
    else if (inputParameters[1] <= 2) value = 32;
    else if (inputParameters[1] <= 3) value = 24;
    else if (inputParameters[1] <= 4) value = 16;
    else if (inputParameters[1] <= 5) value = 8;
    else if (inputParameters[1] <= 6) value = 6;
    else if (inputParameters[1] <= 7) value = 4;
    else if (inputParameters[1] <= 8) value = 3;
    else if (inputParameters[1] <= 9) value = 2;
    else value = 1;

    if (value == -1) delay2 = 0;
    else delay2 = sampleRate / bpm * 60 * 4 / value;

    if (inputParameters[2] == 0) value = -1;
    else if (inputParameters[2] <= 1) value = 64;
    else if (inputParameters[2] <= 2) value = 32;
    else if (inputParameters[2] <= 3) value = 24;
    else if (inputParameters[2] <= 4) value = 16;
    else if (inputParameters[2] <= 5) value = 8;
    else if (inputParameters[2] <= 6) value = 6;
    else if (inputParameters[2] <= 7) value = 4;
    else if (inputParameters[2] <= 8) value = 3;
    else if (inputParameters[2] <= 9) value = 2;
    else value = 1;

    if (value == -1) delay3 = 0;
    else delay3 = sampleRate / bpm * 60 * 4 / value;

    if (inputParameters[3] == 0) value = -1;
    else if (inputParameters[3] <= 1) value = 64;
    else if (inputParameters[3] <= 2) value = 32;
    else if (inputParameters[3] <= 3) value = 24;
    else if (inputParameters[3] <= 4) value = 16;
    else if (inputParameters[3] <= 5) value = 8;
    else if (inputParameters[3] <= 6) value = 6;
    else if (inputParameters[3] <= 7) value = 4;
    else if (inputParameters[3] <= 8) value = 3;
    else if (inputParameters[3] <= 9) value = 2;
    else value = 1;

    if (value == -1) delay4 = 0;
    else delay4 = sampleRate / bpm * 60 * 4 / value;







    feedback = inputParameters[4]/100;

    double cutoff = pow(10, inputParameters[5]/100*(log10(20000)-log10(40))+log10(40));
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
    
    dry = 1 - (inputParameters[6]/100);
    wet = (inputParameters[6]/100);
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
