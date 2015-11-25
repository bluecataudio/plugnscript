/** \file
*   Monoizer.
*   Averages all channels together to create a mono mix,
*   replicated to all outputs.
*/

// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount = 0;
DSP_EXPORT uint    audioOutputsCount = 0;

DSP_EXPORT string description="averages all channels together to create a mono mix";
double norm=1;

DSP_EXPORT bool initialize()
{
    if(audioInputsCount>0)
        norm=1/double(audioInputsCount);
	return true;
}

DSP_EXPORT void processSample(double ioSample[])
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