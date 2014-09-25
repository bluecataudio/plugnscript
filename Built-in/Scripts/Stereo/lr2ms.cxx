string name="LR 2 MS";
string description="Left/Right to Mid/Side";

bool initialize()
{
    // accept only stereo streams
    if(audioInputsCount!=2)
    {
        print("Error: this script requires 2 audio channels, not "+audioInputsCount);
        return false;
    }
    return true;
}

void processSample(array<double>& ioSample)
{  
    double left=ioSample[0];
    double right=ioSample[1];
    ioSample[0]=left+right;
    ioSample[1]=left-right;
}