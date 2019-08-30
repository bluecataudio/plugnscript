/** \file
*   Simple oscilloscope display using output strings.
*/

// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"
#include <string>
DSP_EXPORT uint    audioInputsCount = 0;


DSP_EXPORT string name = "Micr'Oscillo";
DSP_EXPORT string description = "Simple Oscilloscope that displays the last 1024 audio samples (mono)";
DSP_EXPORT array<string> outputStringsNames = { "Audio Data" };
DSP_EXPORT array<int> outputStringsMaxLengths = { 1024 * (24 + 1) }; ///<24 characters required to store doubles as strings
DSP_EXPORT array<string>  outputStrings(outputStringsNames.length, NULL);

// pre-allocated temporary string
std::string tempString;
std::string outputString;

// audio data circular buffer
array<double> buffer(1024);
int mask = 1023;
int currentIndex = 0;
double averageRatio = 1.0;

DSP_EXPORT bool initialize()
{
    tempString.resize(1024);
    outputString.resize(1024 * (24 + 1));
    outputString="";
    if (audioInputsCount != 0)
        averageRatio = 1.0 / double(audioInputsCount);
    return true;
}

DSP_EXPORT void reset()
{
    currentIndex = 0;
    for (uint i = 0; i < 1024; i++)
        buffer[i] = 0;
}

DSP_EXPORT void processSample(double ioSample[])
{
    double value = 0;
    // compute channels average value
    for (uint channel = 0; channel < audioInputsCount; channel++)
    {
        // check if channel is "clipping"
        value += ioSample[channel];
    }
    buffer[currentIndex] = value*averageRatio;
    currentIndex++;
    currentIndex &= mask;
}

DSP_EXPORT void computeOutputData()
{
    // convert raw audio data to csv values (using ';' as separator)
    outputString = "";
    for (int i = 0; i < 1024; i++)
    {
        // convert value to string into our pre-allocated temp string, with re-allocating the string
        double value = buffer[(currentIndex - 1 - i)&mask];
        tempString.resize(1024);
        sprintf(&tempString[0],"%0.6f", value);
        tempString.resize(strlen(&tempString[0]));

        // adding it to the output string
        outputString += tempString;
        if (i != 1023)
            outputString += ";";
    }
    // copy pointer to output
    outputStrings[0] = outputString.c_str();
}