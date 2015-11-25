/** \file
*   audio i/o router that includes aux inuts and outputs.
*   Select source channel for each audio output.
*/

// metadata
string name="i/o router";
string description="Select source for each audio output";

// parameters definition: each parameter correponds to an audio output and lets you choose an input channel
array<string>   inputParametersNames(audioOutputsCount+auxAudioOutputsCount);
array<double>  inputParameters(inputParametersNames.length);
array<double>  inputParametersMin(inputParametersNames.length);
array<double>  inputParametersMax(inputParametersNames.length);
array<double>  inputParametersDefault(inputParametersNames.length);
array<int>      inputParametersSteps(inputParametersNames.length);
array<string>   inputParametersEnums(inputParametersNames.length);

array<int>      sourceChannel(audioOutputsCount+auxAudioOutputsCount);
array<array<double>>   tempInputSamples(audioInputsCount+auxAudioInputsCount+1); // first temp channel is "None" (0)

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

    // initialize parameters properties (depends on the number of i/o channels)
    for(uint i=0, count=inputParameters.length; i<count;i++)
    {
        inputParametersMin[i]=0;
        inputParametersMax[i]=audioInputsCount+auxAudioInputsCount;
        inputParametersSteps[i]=audioInputsCount+auxAudioInputsCount+1;
        if(i<audioOutputsCount)
            inputParametersNames[i]="Out "+(i+1);
        else
            inputParametersNames[i]="Aux Out "+(i-audioOutputsCount+1);
        
        if(i<audioInputsCount)
            inputParametersDefault[i]=i+1;
        else
            inputParametersDefault[i]=0;
        inputParametersEnums[i]="None";
        for(uint ch=0;ch<audioInputsCount+auxAudioInputsCount;ch++)
        {
            inputParametersEnums[i]+=";";
            if(ch<audioInputsCount)
                inputParametersEnums[i]+="Input "+(ch+1);
            else
                inputParametersEnums[i]+="Aux In "+(ch-audioInputsCount+1);            
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
    for(uint ch=0,count=audioInputsCount+auxAudioInputsCount;ch<count;ch++)
    {
        array<double>@ channelSamples=@data.samples[ch];
        array<double>@ tempSamples=@tempInputSamples[ch+1];  
        for(uint i=0;i<data.samplesToProcess;i++)
        {
            tempSamples[i]=channelSamples[i];
        }
    }

    // write to output channels
    for(uint ch=0,count=audioOutputsCount+auxAudioOutputsCount;ch<count;ch++)
    {
        int source=sourceChannel[ch];
        array<double>@ channelSamples=@data.samples[ch];
        array<double>@ tempSamples=@tempInputSamples[sourceChannel[ch]];  
        for(uint i=0;i<data.samplesToProcess;i++)
        {
            channelSamples[i]=0;
            channelSamples[i]=tempSamples[i];
        }
    }
}

void updateInputParametersForBlock(const TransportInfo@ info)
{
    // copy source channels selection
    for(uint channel=0,count=audioOutputsCount+auxAudioOutputsCount;channel<count;channel++)
    {
        sourceChannel[channel]=int(inputParameters[channel]+.5);
    }
}