/** \file
*   Simple oscilloscope display using output strings.
*/

string name = "Micr'Oscillo";
string description = "Simple Oscilloscope that displays the last 1024 audio samples (mono)";
array<string> outputStringsNames = { "Audio Data" };
array<int> outputStringsMaxLengths = { 1024 * (24 + 1) }; ///<24 characters required to store doubles as strings
array<string>  outputStrings(outputStringsNames.length);

// pre-allocated temporary string
string         tempString;

// audio data circular buffer
array<double> buffer(1024);
int mask = 1023;
int currentIndex = 0;
double averageRatio = 1.0;

void initialize()
{
    // pre-allocate our strings
    tempString.resize(1024);
    outputStrings[0].resize(1024 * (24 + 1));
    outputStrings[0]="";
    if (audioInputsCount != 0)
        averageRatio = 1.0 / double(audioInputsCount);
}

void reset()
{
    currentIndex = 0;
    for (uint i = 0; i < 1024; i++)
        buffer[i] = 0;
}

void processSample(array<double>& ioSample)
{
    double value = 0;
    // compute channels average value
    for (uint channel = 0; channel < audioInputsCount; channel++)
    {
        // check if channel is "clipping"
        value += ioSample[channel];
    }
    buffer[currentIndex] = value * averageRatio;
    currentIndex++;
    currentIndex &= mask;
}

void computeOutputData()
{
    // convert raw audio data to csv values (using ';' as separator)
    outputStrings[0] = "";
    for (int i = 0; i < 1024; i++)
    {
        // convert value to string into our pre-allocated temp string, using allocation-free conversion routines
        double value = buffer[(currentIndex - 1 - i) & mask];
        floatToString(value, tempString, "", 0, 6);

        // add it to the output string
        outputStrings[0] += tempString;
        if (i != 1023)
            outputStrings[0] += ";";
    }
}