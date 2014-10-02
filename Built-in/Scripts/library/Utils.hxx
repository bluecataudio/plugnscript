/** 
*  \file Utils.hxx
*  Common math dsp utils for angelscript.
* 
*  Created by Blue Cat Audio <services@bluecataudio.com>
*  Copyright 2011-2014 Blue Cat Audio. All rights reserved.
*
*/

namespace KittyDSP
{
    namespace Utils
    {
        /** Computes the next power of two for an integer.
        *
        */
        int nextPowerOfTwo(int val) 
        {
            val--;
            val = (val >> 1) | val;
            val = (val >> 2) | val;
            val = (val >> 4) | val;
            val = (val >> 8) | val;
            val = (val >> 16) | val;
            val++;
            if(val==0)
                val=2;
            return val;
        }

        /** convert double number to closest integer (rounding).
        *
        */
        int roundDoubleToInt(double d)
        {
            if(d<0)
                return int(d-.5);
            else
                return int(d+.5);
        }
    }
}