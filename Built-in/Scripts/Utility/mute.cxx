/** \file
*   Mute: mutes all audio channels
*/

string description="mute all audio channels";

void processSample(array<double>& ioSample)
{
    // zero outputs
    for(uint i=0;i<audioOutputsCount;i++)
    {
        ioSample[i]=0;
    }
}