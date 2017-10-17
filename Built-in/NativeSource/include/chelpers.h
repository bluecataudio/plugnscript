/**
 *  C Helper structs and functions to build dsp scripts.
 *  Copyright (c) 2015-2017 Blue Cat Audio. All rights reserved.
 */

#ifndef _chelpers_h
#define _chelpers_h

#include "dspapi.h"

/** C Array Descriptor expected by host for arrays of strings.
 *  (inputParametersNames, inputParametersUnits, etc.)
 */
struct CStringArray
{
    char const** ptr;
    uint         length;
};


/** C Array Descriptor expected by host for arrays of double values.
 *  (inputParameters, outputParameters etc.)
 */
struct CDoubleArray
{
    double*     ptr;
    uint        length;
};

/** C Array Descriptor expected by host for arrays of integer values.
 *  (inputParametersSteps)
 */
struct CIntArray
{
    int*        ptr;
    uint        length;
};

#endif
