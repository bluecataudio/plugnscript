/** \file
 *   Multichannel peak levels meter.
 */

// Input Parameters
array<string> inputParametersNames={"Scale","Decay"};
array<double> inputParametersDefault={1.0,.3};
array<double> inputParametersMax={1.0};
array<int> inputParametersSteps={2,-1};
array<string> inputParametersEnums={"Linear;Log(dB)"};
array<double> inputParameters(inputParametersNames.length);

// Output parameters definition
array<string> outputParametersNames={"Level"};
array<string> outputParametersUnits={"%"};
array<double> outputParameters(outputParametersNames.length);
array<double> outputParametersMin={0};
array<double> outputParametersMax={1};

// meter decay
double decay=0;

// the measured level
double level=0;

/* per-sample processing.
 *
 */
void processSample(array<double>& ioSample)
{
    double maxValue=0;
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        double value=abs(ioSample[channel]);
        if(value>maxValue)
            maxValue=value;
    }
    if(maxValue>level)
        level=maxValue;
    else
        level+=decay*(maxValue-level);
}

int getTailSize()
{
    // 1000 milliseconds to let meters reach -60 dB
    return int(1*sampleRate);
}

void reset()
{
    // reset level to 0
    level=0;
}

void updateInputParametersForBlock(const TransportInfo@ info)
{
    decay=(1-exp(-log(10)/(sampleRate*.1*(.01+5*inputParameters[1]))));
}

void computeOutputData()
{
    // linear mode
    if(inputParameters[0]<.5)
        outputParameters[0]=level;
    else
    {
        // Log scale (dB)
        if(level>pow(10,-3)) // -60 dB limit
            outputParameters[0]=(20.0*log10(level)+60.0)/60.0;
        else
            outputParameters[0]=0;
    }
}