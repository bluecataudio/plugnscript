/** \file
*   Monoizer.
*   Averages all channels together to create a mono mix,
*   replicated to all outputs.
*/

string description="averages all channels together to create a mono mix";
double norm=1;

void initialize()
{
    if(audioInputsCount>0)
        norm=1/double(audioInputsCount);
}

void processSample(array<double>& ioSample)
{
    // compute average
    double sum=0;
    for(uint i=0;i<audioInputsCount;i++)
    {
        sum+=ioSample[i];
    }
    sum*=norm;
    
    // copy to outputs
    for(uint i=0;i<audioOutputsCount;i++)
    {
        ioSample[i]=sum;
    }
}