/** \file
*   Simple sawtooth generator.
*   Charles Verron - 2016 - Public domain
*/

string name="Simple sawtooth";
string description="Simple sawtooth generator";

array<string> inputParametersNames={"Amplitude", "Frequency"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersDefault={.5, .5};

double amplitude, freqNormalized;
double sampleValue = 0;
double minFreqHz = 50;
double maxFreqHz = 1000;
double freqRange = maxFreqHz - minFreqHz;

void processSample(array<double>& ioSample)
{
    sampleValue += freqNormalized;
    if (sampleValue >= .5) // Threshold 0.5 -> No DC, or 1.0 -> DC
        sampleValue -= 1.;
    
    // copy to all audio outputs
    for(uint channel=0;channel<audioOutputsCount;channel++)
    {
        ioSample[channel]=sampleValue*amplitude;
    }
}

void updateInputParameters()
{
   amplitude=inputParameters[0]*.5; 
   freqNormalized=(inputParameters[1]*freqRange + minFreqHz)/sampleRate;
}

int getTailSize()
{
    return -1;
}
