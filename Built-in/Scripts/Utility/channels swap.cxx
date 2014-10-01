/** \file
*   Multiple Channels Swapper.
*   Select source channel for each audio output.
*/

// metadata
string name="Multi Channel Swapper";
string description="Select source channel for each audio output";

// parameters definition: each parameter correponds to an audio output and lets you choose an input channel
array<string>   inputParametersNames(audioOutputsCount);
array<double>  inputParameters(inputParametersNames.length);
array<double>  inputParametersMin(inputParametersNames.length);
array<double>  inputParametersMax(inputParametersNames.length);
array<double>  inputParametersDefault(inputParametersNames.length);
array<int>      inputParametersSteps(inputParametersNames.length);
array<string>   inputParametersEnums(inputParametersNames.length);

array<int>      sourceChannel(audioOutputsCount);
array<array<double>>   tempInputSamples(audioInputsCount+1); // first temp channel is "None" (0)

void initialize()
{
    // allocate using the maximum block size, if known
    if(maxBlockSize>0)
    {
        for(uint i=0;i<tempInputSamples.length;i++)
        {
            tempInputSamples[i].length=maxBlockSize;
        }
    }

    // set the first input samples to 0
    for(uint i=0;i<tempInputSamples[0].length;i++)
    {
        tempInputSamples[0][i]=0;
    }

    // initialize parameters properties (depend on the number of i/o channels)
    for(uint i=0; i<audioOutputsCount;i++)
    {
        inputParametersMin[i]=0;
        inputParametersMax[i]=audioInputsCount;
        inputParametersSteps[i]=audioInputsCount+1;

        inputParametersNames[i]="Out "+(i+1);
        if(i<=audioInputsCount)
            inputParametersDefault[i]=i+1;
        else
            inputParametersDefault[i]=0;
        inputParametersEnums[i]="None";
        for(uint ch=0;ch<audioInputsCount;ch++)
        {
            inputParametersEnums[i]+=";";
            inputParametersEnums[i]+="Channel "+(ch+1);
        }
    }
}

void processBlock(BlockData& data)
{
    // make sure our buffers have the appropriate size (may have not been initialized if maxBlockSize was not set at startup)
    {
        const uint allocatedBufferLength=tempInputSamples[0].length;
        if(data.samplesToProcess>allocatedBufferLength)
        {
            uint oldLength=tempInputSamples[0].length;
            for(uint i=0;i<tempInputSamples.length;i++)
            {
                tempInputSamples[i].length=data.samplesToProcess;
            }
            // init additional first buffer samples to zero
            for(uint i=allocatedBufferLength;i<data.samplesToProcess;i++)
            {
                tempInputSamples[0][i]=0;
            }
        }
    }

    // copy samples to temp buffers
    for(uint ch=0;ch<audioOutputsCount;ch++)
    {
        array<double>@ channelSamples=@data.samples[ch];
        array<double>@ tempSamples=@tempInputSamples[ch+1];  
        for(uint i=0;i<data.samplesToProcess;i++)
        {
            tempSamples[i]=channelSamples[i];
        }
    }

    // write to output channels
    for(uint ch=0;ch<audioOutputsCount;ch++)
    {
        int source=sourceChannel[ch];
        array<double>@ channelSamples=@data.samples[ch];
        array<double>@ tempSamples=@tempInputSamples[sourceChannel[ch]];  
        for(uint i=0;i<data.samplesToProcess;i++)
        {
            channelSamples[i]=tempSamples[i];
        }
    }
}

void updateInputParametersForBlock(const TransportInfo@ info)
{
    // copy source channels selection
    for(uint channel=0;channel<audioOutputsCount;channel++)
    {
        sourceChannel[channel]=int(inputParameters[channel]+.5);
    }
}