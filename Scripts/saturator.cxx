/**  
*   Made by Michael Donovan 
*   (https://github.com/michaeldonovan)
*/

/** Script metadata.
*/
string name="Saturator";

/** Define our parameters.
*/
array<string> inputParametersNames={"Drive", "Shape", "Output"};
array<string> inputParametersUnits={"dB", "", "dB"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersMin={-20, 1, -20};
array<double> inputParametersMax={20, 10, 20};
array<double> inputParametersDefault={0, 5, 0};

/** Define our internal variables.
*
*/
double preGain=0;
double cliplevel=1;
double postGain=0;
double c=1;

/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{  
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        double sample=ioSample[channel];
        sample*=preGain;

        int sign = 1;
        if(sample<0)
            sign=-1;

        sample = (1/ c) * fastAtan(sample *  c);

        sample*=postGain;

        ioSample[channel]=sample;
    }
}

double fastAtan(double x)
{
    return (x / (1.0 + 0.28 * (x * x)));
}

/** update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    preGain=pow(10, inputParameters[0]/20);
    c=inputParameters[1];
    postGain=pow(10,inputParameters[2]/20);
}
