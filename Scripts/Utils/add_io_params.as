///////////////////////////
// This is a helper library for Plug-n-Script
// for more convenient way of adding
// input/output params and input/output strings
// for AngelScript version
///////////////////////////
// written by Ilya Orlov (LetiMix) | ilya2k@gmail.com
// 2020 January-April | St. Petersburg, Russia

// HOW TO USE

// EXAMPLE 1

// #include "_lib/add_io_params.as"

// // enum is alternative to writing "const int ENABLE = 0, MODE = 1 ..."
// enum inputParamsEnum{
//     ENABLE,
//     MODE,
//     THRESHOLD,
//     MIX,
// }
// enum outParamsEnum{
//     GR,
//     OUT_MODE,
//     OUT_LEVEL
// }
// enum inStrings{
//     FILENAME
// }
// enum outStrings{
//     MYDATA1
// }

// void initialize(){

//     // format for adding params
//     // index, "name", ["units", min, max, default, "format", [steps]] 
//     // format for adding enumerated values params
//     // index, "name", "value1;value2;value3", ["default_value"]

//     // add input params
//     ip(ENABLE, "Enable", "Off;On");
//     ip(MODE, "Mode", "Simple;Classic;1176;Opto", "Classic");
//     ip(THRESHOLD, "Threshold", "dB", -40, 0, -18, ".1");
//     ip(MIX, "Wet/Dry");

//     // add output params
//     op(GR, "Gain Reduction", "db", 0, 10);
//     op(OUT_MODE, "Out mode", "Simple;Classic;1176;Opto");
//     op(OUT_LEVEL, "Out level");
    
//     // add input strings
//     ips(FILENAME, "Filename");

//     // add output strings
//     ops(MYDATA1, "MyData1", 256);
// }


// EXAMPLE 2

// instead of creating constants and passing them to functions,
// you can add get params index from functions

// // create placeholders for params index
// int ENABLE, MODE, THRESHOLD, MIX, GR, OUT_MODE, OUT_LEVEL, FILENAME, MYDATA1;

// void initialize(){
//     // format for adding params
//     // index, "name", ["units", min, max, default, "format", [steps]] 
//     // format for adding enumerated values params
//     // index, "name", "value1;value2;value3", ["default_value"]

//     // add input params
//     ENABLE = ip("Enable", "Off;On");
//     MODE = ip("Mode", "Simple;Classic;1176;Opto", "Classic");
//     THRESHOLD = ip("Threshold", "dB", -40, 0, -18, ".1");
//     MIX = ip("Wet/Dry");

//     // add output params
//     GR = op("Gain Reduction", "db", 0, 10);
//     OUT_MODE = op("Out mode", "Simple;Classic;1176;Opto");
//     OUT_LEVEL = op("Out level");
    
//     // add input strings
//     FILENAME = ips("Filename");

//     // add output strings (with max length)
//     MYDATA1 = ops("MyData1", 256);
// }


// EXAMPLE 3

// You can use shortcuts IP (for inputParameters), OP (for outputParameters)
// IPS and OPS (for inputStrings and outputStrings)

// void updateInputParameters() {
//     double threshold = IP[THRESHOLD];
//     string mystring = IPS[FILENAME];
// }

// void computeOutputData(){
//     OP[GR] = rand(0,10);
// }



// create arrays for input/output params and strings
array<double> inputParameters, outputParameters,
    inputParametersMin, outputParametersMin,
    inputParametersMax, outputParametersMax,
    inputParametersDefault, outputParametersDefault; 
array<string> inputParametersNames, outputParametersNames,
    inputParametersUnits, outputParametersUnits,
    inputParametersFormats, outputParametersFormats,
    inputParametersEnums, outputParametersEnums,
    inputStrings, outputStrings, inputStringsNames, outputStringsNames;
array<int> inputParametersSteps, outputStringsMaxLengths;

// shortcuts for input/output parameters and strings
array<double>@ IP = inputParameters; 
array<double>@ OP = outputParameters; 
array<string>@ IPS = inputStrings;
array<string>@ OPS = outputStrings;

// INPUT PARAMETERS

// add (or update) input parameter
void ip(int index, string name, string units, double min, double max = 0, double def_no = 0,  string vf = ".1", int steps = 0, string enums = "") {
    if (index < 0) index = int(inputParameters.length); // auto-increase index
    uint nn = index+1;
    // verify arrays size
    if (inputParameters.length < nn) {
        inputParameters.resize(nn);
        inputParametersNames.resize(nn);
        inputParametersUnits.resize(nn);
        inputParametersMin.resize(nn);
        inputParametersMax.resize(nn);
        inputParametersDefault.resize(nn);
        inputParametersFormats.resize(nn);
        inputParametersSteps.resize(nn);
        inputParametersEnums.resize(nn);
    }
    // add param name (name)
    inputParametersNames[index] = name;
    inputParametersUnits[index] = units;
    inputParametersMin[index] = min;
    inputParametersMax[index] = max;
    inputParametersDefault[index] = def_no;
    inputParametersFormats[index] = vf;
    // auto calculation of steps if value_format = .0
    if ((steps < 1) and ((parseFloat(vf) - floor(parseFloat(vf))) == 0)) { 
        steps = int(max - min) + 1;
    }
    inputParametersSteps[index] = steps;
    inputParametersEnums[index] = enums;
}

// add or update enumerated input parameter
void ip(int index, string name, string enums = "%", string def_value = ""){
    // check if we're adding enums
    if (enums.findFirst(";") > 0) {
        array<string> ar = enums.split(";");
        int max = ar.length - 1;
        int def_no = 0; // find index of default_value
        if (def_value != "") def_no = ar.find(def_value);
        if (def_no < 0) def_no = 0;
        // add enum
        ip(index, name, "", 0, max, def_no, "", ar.length, enums);
    } else {
        // add regular param
        string units = enums;
        ip(index, name, units, 0);
    }
}

// the same but adding item to the end of list and returning index
int ip(string name, string units, double min, double max = 0, double def_no = 0,  string vf = ".1", int steps = 0, string enums = ""){
    int index = int(inputParameters.length);
    ip(index, name, units, min, max, def_no, vf, steps, enums);
    return index;
}
// adding enum param to end of list and returning index
int ip(string name, string enums = "%", string def_value = ""){
    int index = int(inputParameters.length);
    ip(index, name, enums, def_value);
    return index;
}

// OUTPUT PARAMETERS

// add (or update) output parameter
void op(int index, string name, string units, double min, double max = 0, double def_no = 0,  string vf = ".1", string enums = "") {
    uint nn = index+1;
    // verify arrays size
    if (outputParameters.length < nn) {
        outputParameters.resize(nn);
        outputParametersNames.resize(nn);
        outputParametersUnits.resize(nn);
        outputParametersMin.resize(nn);
        outputParametersMax.resize(nn);
        outputParametersDefault.resize(nn);
        outputParametersFormats.resize(nn);
        outputParametersEnums.resize(nn);
    }
    // add param name (name)
    outputParametersNames[index] = name;
    outputParametersUnits[index] = units;
    outputParametersMin[index] = min;
    outputParametersMax[index] = max;
    outputParametersDefault[index] = def_no;
    outputParametersFormats[index] = vf;
    outputParametersEnums[index] = enums;
}

// add or update enumerated output parameter
void op(int index, string name, string enums = "%", string def_value = ""){
    // check if we're adding enums
    if (enums.findFirst(";") > 0) {
        array<string> ar = enums.split(";");
        int max = ar.length - 1;
        int def_no = 0; // find index of default_value
        if (def_value != "") def_no = ar.find(def_value);
        if (def_no < 0) def_no = 0;
        // add enum
        op(index, name, "", 0, max, def_no, "", enums);
    } else {
        // add regular param
        string units = enums;
        op(index, name, units, 0);
    }
}
// the same but adding item to the end of list and returning index
int op(string name, string units, double min, double max = 0, double def_no = 0,  string vf = ".1", string enums = ""){
    int index = int(outputParameters.length);
    op(index, name, units, min, max, def_no, vf, enums);
    return index;
}
// adding enum param to end of list and returning index
int op(string name, string enums = "%", string def_value = ""){
    int index = int(outputParameters.length);
    op(index, name, enums, def_value);
    return index;
}

// INPUT STRINGS

// add/update input string via index
void ips(int index, string name) {
    uint nn = index+1;
    // verify arrays size
    if (inputStrings.length < nn) {
        inputStrings.resize(nn);
        inputStringsNames.resize(nn);
    }
    inputStringsNames[index] = name;
}

// adding input string to the end of list and returning index
int ips(string name){
    int index = int(inputStrings.length);
    ips(index, name);
    return index;
}

// OUTPUT STRINGS

// add/update output string via index
void ops(int index, string name, int maxlen = 1024) {
    uint nn = index+1;
    // verify arrays size
    if (outputStrings.length < nn) {
        outputStrings.resize(nn);
        outputStringsNames.resize(nn);
        outputStringsMaxLengths.resize(nn);
    }
    outputStringsNames[index] = name;
    outputStringsMaxLengths[index] = maxlen;
}

// adding output string to the end of list and returning index
int ops(string name, int len = 1024){
    int index = int(outputStrings.length);
    ops(index, name);
    return index;
}
