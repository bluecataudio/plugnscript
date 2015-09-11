/** \file
*   Multichannel gain.
*   Apply different gain to individual channels.
*/
string description="multi channel gain";

array<string> inputParametersNames(audioInputsCount);
array<double> inputParameters(inputParametersNames.length);
array<double> gain(inputParametersNames.length);

void initialize()
{
    if(audioInputsCount==1)
        inputParametersNames[0]="Gain";
    else if(audioInputsCount==2)
    {
        inputParametersNames[0]="Left";
        inputParametersNames[1]="Right";
    }
    else
    {
        for(uint i=0;i<inputParametersNames.length;i++)
        {
            inputParametersNames[i]="Ch"+(i+1);
        }
    }
}

void processSample(array<double>& ioSample)
{
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        ioSample[channel]*=gain[channel];
    }
}

void updateInputParameters()
{
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        gain[channel]=inputParameters[channel];
    }
}