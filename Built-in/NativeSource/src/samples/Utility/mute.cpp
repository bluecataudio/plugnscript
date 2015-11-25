/** \file
*   Mute: mutes all audio channels
*/

// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

// variables filled by host
DSP_EXPORT uint    audioOutputsCount = 0;


DSP_EXPORT string description="mute all audio channels";

DSP_EXPORT void processSample(double ioSample[])
{
    // zero outputs
    for(uint i=0;i<audioOutputsCount;i++)
    {
        ioSample[i]=0;
    }
}