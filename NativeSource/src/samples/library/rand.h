#ifndef _rand_h_
#define _rand_h_

// Mersenne Twister implementation for pseudo random numbers generation
// http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/emt19937ar.html
// used instead of libc rand function because processing may occur in multiple threads
#include <time.h>

#define RANDOM_N 624
#define RANDOM_M 397
#define RANDOM_MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define RANDOM_UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define RANDOM_LOWER_MASK 0x7fffffffUL /* least significant r bits */

struct RandomGen
{
    unsigned long mt[624];
    int mti;
 
    RandomGen()
    {
        init_genrand((unsigned long)time(NULL));
    }
    
    /* initializes mt[RANDOM_N] with a seed */
    void init_genrand(unsigned long s)
    {
        mt[0]= s & 0xffffffffUL;
        for (mti=1; mti<RANDOM_N; mti++) {
            mt[mti] =
            (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
            /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
            /* In the previous versions, MSBs of the seed affect   */
            /* only MSBs of the array mt[].                        */
            /* 2002/01/09 modified by Makoto Matsumoto             */
            mt[mti] &= 0xffffffffUL;
            /* for >32 bit machines */
        }
    }
    
    /* generates a random number on [0,0xffffffff]-interval */
    unsigned long genrand_int32(void)
    {
        unsigned long y;
        static unsigned long mag01[2]={0x0UL, RANDOM_MATRIX_A};
        /* mag01[x] = x * RANDOM_MATRIX_A  for x=0,1 */
        
        if (mti >= RANDOM_N) { /* generate RANDOM_N words at one time */
            int kk;
            
            if (mti == RANDOM_N+1)   /* if init_genrand() has not been called, */
                init_genrand(5489UL); /* a default initial seed is used */
            
            for (kk=0;kk<RANDOM_N-RANDOM_M;kk++) {
                y = (mt[kk]&RANDOM_UPPER_MASK)|(mt[kk+1]&RANDOM_LOWER_MASK);
                mt[kk] = mt[kk+RANDOM_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
            }
            for (;kk<RANDOM_N-1;kk++) {
                y = (mt[kk]&RANDOM_UPPER_MASK)|(mt[kk+1]&RANDOM_LOWER_MASK);
                mt[kk] = mt[kk+(RANDOM_M-RANDOM_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
            }
            y = (mt[RANDOM_N-1]&RANDOM_UPPER_MASK)|(mt[0]&RANDOM_LOWER_MASK);
            mt[RANDOM_N-1] = mt[RANDOM_M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];
            
            mti = 0;
        }
        
        y = mt[mti++];
        
        /* Tempering */
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680UL;
        y ^= (y << 15) & 0xefc60000UL;
        y ^= (y >> 18);
        
        return y;
    }
};

/** Pseudo random numbers generation.
 *
 */
double rand(double min, double max)
{
    static RandomGen generator;
    return (double(generator.genrand_int32())/double(4294967295.0))*(max-min)+min;
}

#endif