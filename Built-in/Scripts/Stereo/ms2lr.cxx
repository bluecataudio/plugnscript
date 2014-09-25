string name="MS 2 LR";
string description="Mid/Side to Left/Right";

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
    double mid=ioSample[0];
    double side=ioSample[1];
    ioSample[0]=.5*(mid+side);
    ioSample[1]=.5*(mid-side);
}