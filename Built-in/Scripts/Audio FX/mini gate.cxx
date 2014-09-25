string name="Mini Gate";
string description="mini gate";

/** Define our parameters.
*/
array<string>  inputParametersNames={"Threshold","Ratio", "Attack", "Release"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersDefault={0,0,.1,.5};
array<string>  outputParametersNames={"Comp"};
array<double> outputParameters(outputParametersNames.length);

// internal variables
double threshold=1;
double ratio=1;
double level=0;
double attackCoeff=0;
double releaseCoeff=0;
double gain=0;

void processSample(array<double>& ioSample)
{
    // compute average absolute value
    double absValue=0;
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        absValue+=abs(ioSample[channel]);
    }
    absValue/=double(audioInputsCount);

    // update level
    if(absValue>level)
        level+=attackCoeff*(absValue-level);
    else
        level+=releaseCoeff*(absValue-level);

    // apply ratio if level below threshold
    gain=1;
    if(level<threshold && level>1e-20)
    {
        gain=(ratio*(level-threshold)+threshold)/level;
        if(gain<0)
            gain=0;
        for(uint channel=0;channel<audioInputsCount;channel++)
        {
            ioSample[channel]*=gain;
        }
    }
}

void updateInputParametersForBlock(const TransportInfo@ )
{
    threshold=pow(10,3*(inputParameters[0]-1)); // -60 to 0 dB
    ratio=(1+9*(exp(log(101)*inputParameters[1])-1)/100);
    attackCoeff=pow(10,1.0/(50+.5*sampleRate*inputParameters[2]))-1;
    releaseCoeff=pow(10,1.0/(50+.5*sampleRate*inputParameters[3]))-1;
}

void computeOutputData()
{
    // display compression from 0 to 60 dB
    double compGain=1;
    if(gain>0)
    {
        compGain=20*log(gain);
        if(compGain<-60)
            compGain=-60;
        compGain/=-60;
    }
    outputParameters[0]=compGain;
}