// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount=0;
DSP_EXPORT void*   host=null;
DSP_EXPORT HostPrintFunc* hostPrint=null;

#include <string>

/** \file
*   Left/Right to Mid/Side Transform.
*   For stereo audio streams only (2 channels).
*/

DSP_EXPORT string name="LR 2 MS";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Left/Right to Mid/Side";

DSP_EXPORT bool initialize()
{
    // accept only stereo streams
    if(audioInputsCount!=2)
    {
        std::string message="Error: this script requires 2 audio channels, not "+std::to_string(audioInputsCount);
        print(message.c_str());
        return false;
    }
    return true;
}

DSP_EXPORT void processSample(double ioSample[])
{  
    double left=ioSample[0];
    double right=ioSample[1];
    ioSample[0]=left+right;
    ioSample[1]=left-right;
}