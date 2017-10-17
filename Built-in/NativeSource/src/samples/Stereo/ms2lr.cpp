// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount=0;
DSP_EXPORT void*   host=null;
DSP_EXPORT HostPrintFunc* hostPrint=null;

#include <string>

/** \file
*   Mid/Side to Left/Right Transform.
*   For stereo audio streams only (2 channels).
*/

DSP_EXPORT string name="MS 2 LR";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Mid/Side to Left/Right";

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
    double mid=ioSample[0];
    double side=ioSample[1];
    ioSample[0]=.5*(mid+side);
    ioSample[1]=.5*(mid-side);
}