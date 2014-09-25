string name="Stereo Invert";
string description="Invert left and right channels";

bool initialize()
{
    bool ok=true;
    // accept only stereo streams
    if(audioInputsCount!=2)
    {
        print("Error: this script requires 2 audio channels, not "+audioInputsCount);
        ok=false;
    }
    return ok;
}

void processSample(array<double>& ioSample)
{  
    double temp=ioSample[0];
    ioSample[0]=ioSample[1];
    ioSample[1]=temp;
}