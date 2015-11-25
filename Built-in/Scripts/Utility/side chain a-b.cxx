/** \file
 *   Monitor side chain or main input.
 */

string name="Side Chain A/B";
string description="A/B with Side Chain";

array<string> inputParametersNames={"Source"};
array<double> inputParametersMin={0};
array<double> inputParametersMax={1};
array<int>    inputParametersSteps={2};
array<string> inputParametersEnums={"Main;Side Chain"};
array<double> inputParameters(inputParametersNames.length);

double inputGain=0;
double sideChainGain=0;
uint    commonInputsCount=audioInputsCount;

void initialize()
{
    // compute common number of channels
    if(auxAudioInputsCount<commonInputsCount)
        commonInputsCount=auxAudioInputsCount;
}

void processSample(array<double>& ioSample)
{
    for(uint channel=0;channel<commonInputsCount;channel++)
    {
        ioSample[channel]=ioSample[channel]*inputGain+ioSample[channel+audioInputsCount]*sideChainGain;
    }
    for(uint channel=commonInputsCount;channel<audioInputsCount;channel++)
    {
        ioSample[channel]*=inputGain;
    }
}

void updateInputParameters()
{
    sideChainGain=inputParameters[0];
    inputGain=1-sideChainGain;
}