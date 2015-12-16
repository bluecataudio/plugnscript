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
array <string> inputParametersNames={"Decay","Size","Feedback","Dry/Wet"};
array <string> inputParametersUnits={"%","%","%","%"};
array <double> inputParameters(inputParametersNames.length);
array <double> inputParametersDefault ={  0,   0,   0,   0};
array <double> inputParametersMin     ={  0,   0,   0,   0};
array <double> inputParametersMax     ={100, 100, 100, 100};

string name = "Reverb JCREV";
string author = "Ivan COHEN";
string description = "Schroeder's reverberation algorithm with 4 comb + 3 allpass filters";

// Define our internal variables.
double dry, wet;
double global_feedback;

uint delayc1, delayc2, delayc3, delayc4;
uint delaya1, delaya2, delaya3;

double feedbackc1, feedbackc2, feedbackc3, feedbackc4;
double feedbacka1, feedbacka2, feedbacka3;

int cpt;

// Classes definition
class sampleProcessor
{
    array <double> bufferc1(DELAY_WIDTH);
    array <double> bufferc2(DELAY_WIDTH);
    array <double> bufferc3(DELAY_WIDTH);
    array <double> bufferc4(DELAY_WIDTH);

    array <double> buffera1(DELAY_WIDTH);
    array <double> buffera2(DELAY_WIDTH);
    array <double> buffera3(DELAY_WIDTH);

    double yold;

    void processSample(double& sample)
    {
        const double input = sample + global_feedback * yold;

        // Serial Schroeder's allpass filters
        // -------------------------------------------------------------------------------------------
        const double tempa1 = buffera1[(cpt+delaya1) & (DELAY_WIDTH-1)];
        buffera1[cpt] = input + feedbacka1*tempa1;
        const double ya1 = -feedbacka1*buffera1[cpt] + tempa1;

        const double tempa2 = buffera2[(cpt+delaya2) & (DELAY_WIDTH-1)];
        buffera2[cpt] = ya1 + feedbacka2*tempa2;
        const double ya2 = -feedbacka2*buffera2[cpt] + tempa2;

        const double tempa3 = buffera3[(cpt+delaya3) & (DELAY_WIDTH-1)];
        buffera3[cpt] = ya2 + feedbacka3*tempa3;
        const double ya3 = -feedbacka3*buffera3[cpt] + tempa3;
            
        // Parallel Comb Filters
        // -------------------------------------------------------------------------------------------
        const double yc1 = bufferc1[(cpt+delayc1) & (DELAY_WIDTH-1)];
        const double yc2 = bufferc2[(cpt+delayc2) & (DELAY_WIDTH-1)];
        const double yc3 = bufferc3[(cpt+delayc3) & (DELAY_WIDTH-1)];
        const double yc4 = bufferc4[(cpt+delayc4) & (DELAY_WIDTH-1)];

        bufferc1[cpt] = ya3 - yc1*feedbackc1;
        bufferc2[cpt] = ya3 - yc2*feedbackc2;
        bufferc3[cpt] = ya3 - yc3*feedbackc3;
        bufferc4[cpt] = ya3 - yc4*feedbackc4;
            
        sample = bufferc1[cpt]+bufferc2[cpt]+bufferc3[cpt]+bufferc4[cpt];
        yold = sample;
            
        sample = sample * wet + input * dry;

    }

    void reset()
    {  	
        yold = 0;

        for(uint i=0; i<DELAY_WIDTH; i++)
        {
            bufferc1[i] = 0;
            bufferc2[i] = 0;
            bufferc3[i] = 0;
            bufferc4[i] = 0;

            buffera1[i] = 0;
            buffera2[i] = 0;
            buffera3[i] = 0;
        }    
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
    dry = 1 - (inputParameters[3]/100) * (inputParameters[3]/100) * 0.5;
    wet = 0.3 * inputParameters[3]/100;

    delayc1 = 1687 + uint(floor(1.6*inputParameters[0]-800));
    feedbackc1 = 0.773 * (2*inputParameters[1]/100); if (feedbackc1 > 0.9) feedbackc1 = 0.9;

    delayc2 = 1601 + uint(floor(1.6*inputParameters[0]-800));
    feedbackc2 = 0.802 * (2*inputParameters[1]/100); if (feedbackc2 > 0.9) feedbackc2 = 0.9;

    delayc3 = 2053 + uint(floor(1.6*inputParameters[0]-800)); 
    feedbackc3 = 0.753 * (2*inputParameters[1]/100); if (feedbackc3 > 0.9) feedbackc3 = 0.9;

    delayc4 = 2251 + uint(floor(1.6*inputParameters[0]-800));
    feedbackc4 = 0.733 * (2*inputParameters[1]/100); if (feedbackc4 > 0.9) feedbackc4 = 0.9;

    delaya1 = 347; feedbacka1 = 0.7;
    delaya2 = 113; feedbacka2 = 0.7;
    delaya3 = 37;  feedbacka3 = 0.7;

    global_feedback = inputParameters[2]*2.5/100/100;
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