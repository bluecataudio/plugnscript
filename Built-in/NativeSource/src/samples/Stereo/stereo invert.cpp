// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount=0;
DSP_EXPORT void*   host=null;
DSP_EXPORT HostPrintFunc* hostPrint=null;

#include <string>

/** \file
*   Left/Right channels permutation.
*   For stereo audio streams only (2 channels).
*/

DSP_EXPORT string name="Stereo Invert";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Invert left and right channels";

DSP_EXPORT bool initialize()
{
    bool ok=true;
    // accept only stereo streams
    if(audioInputsCount!=2)
    {
        std::string message="Error: this script requires 2 audio channels, not "+std::to_string(audioInputsCount);
        print(message.c_str());
        ok=false;
    }
    return ok;
}

DSP_EXPORT void processSample(double ioSample[])
{  
    double temp=ioSample[0];
    ioSample[0]=ioSample[1];
    ioSample[1]=temp;
}