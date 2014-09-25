// Output parameters definition
array<string> outputParametersNames(audioInputsCount);
array<string> outputParametersUnits(audioInputsCount);
array<double> outputParameters(audioInputsCount);
array<double> outputParametersMin(audioInputsCount);
array<double> outputParametersMax(audioInputsCount);

// meter decay
const double decay=1-exp(-log(10)/(sampleRate*.3));

void initialize()
{
    // initialize output parameters properties
    if(audioInputsCount==1)
        outputParametersNames[0]="Level";
    else if(audioInputsCount==2)
    {
        outputParametersNames[0]="Left";
        outputParametersNames[1]="Right";
    }
    else
    {
        for(uint i=0;i<outputParametersNames.length;i++)
        {
            outputParametersNames[i]="Level Ch"+(i+1);
        }
    }
    for(uint i=0;i<audioInputsCount;i++)
    {
        outputParametersUnits[i]="dB";
        outputParametersMin[i]=-60;
        outputParametersMax[i]=0;
    } 
}

// levels array
array<double>   levels(audioInputsCount);

/** per-sample processing.
*
*/
void processSample(array<double>& ioSample)
{
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        double value=abs(ioSample[channel]);
        double level=levels[channel];
        if(value>level)
            level=value;
        else
            level+=decay*(value-level);
        levels[channel]=level;
    }
}

int getTailSize()
{
    // 1000 milliseconds to let meters reach -60 dB
    return int(1*sampleRate);
}

void reset()
{
    // reset levels to 0
    for(uint i=0;i<levels.length;i++)
    {
        levels[i]=0;
    }
}

void computeOutputData()
{
    for(uint i=0;i<audioInputsCount;i++)
    {
        if(levels[i]>pow(10,-3)) // -60 dB limit
            outputParameters[i]=20*log10(levels[i]);
        else
            outputParameters[i]=-60;
    }
}