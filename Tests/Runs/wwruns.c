/*
 * wwruns.c
 *
 * Created: 06. Sep. 2022
 *  Author: Frank Bjørnø
 *
 * Purpose: 
 *      To test for randomness in a sequence of numbers using the Wald-Wolfowits runs test.
 *
 * Compilation:
 *     From the command line with Microsoft (R) Macro Assembler and 
 *                                Microsoft (R) C/C++ Optimizing Compiler.
 *
 *      1. Assemble rng.asm without linking using the command
 *         ml /c rng.asm
 *      2. Compile and link the program using the command
 *         cl wwruns.c statistics.c rng.obj
 *
 * License:
 * 
 *          Copyright (C) 2022 Frank Bjørnø
 *
 *          1. Permission is hereby granted, free of charge, to any person obtaining a copy 
 *          of this software and associated documentation files (the "Software"), to deal 
 *          in the Software without restriction, including without limitation the rights 
 *          to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
 *          of the Software, and to permit persons to whom the Software is furnished to do 
 *          so, subject to the following conditions:
 *        
 *          2. The above copyright notice and this permission notice shall be included in all 
 *          copies or substantial portions of the Software.
 *
 *          3. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 *          INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 *          PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 *          HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
 *          CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
 *          OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */



#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "statistics.h"


#define SEQUENCE   40
#define ALPHA       0.05


extern         void randomize(void);
extern unsigned int rndmax();
extern unsigned int rnd();



int main(void)
{
	randomize();		
	
	printf("\n\n\n               Wald-Wolfowitz runs test.\n\n\n   ");	
	
		//  declare and initialize variables used in the first step.
	double   mean = (rndmax() / 2.0);
	uint8_t  current = 2;								//  initialize to "impossible" value
	uint8_t  runs    = 0;
	uint8_t  ones    = 0;
	uint8_t  value;
	
		//  Count ones and runs in a sequence of numbers generated by the RNG
		//  If a number is higher than mean, count a 1.
	for (int c = 0; c < SEQUENCE; c++)
	{
		value = (rnd() > mean);							//  get a 1 or a zero depending on the generated number
		ones += value;								//  count ones
		
		printf(" %c ", value ? '+' : '-');					//  print sequence
		
		if (value != current)							//  count runs
		{
			runs++;
			current = value;
		}
	}
	
		//  calculate expected runs and standard deviation
	double    m = ones;								//  ones
	double    n = SEQUENCE - ones;							//  zeros
	double _2mn = 2.0 * m * n;							//  simplifies calculations
	double   mn = m + n;								//  simplifies calculations					
	
	double ER = (_2mn / (mn)) + 1;							//  Expected number of runs
	double VR = (_2mn * (_2mn - mn)) / (mn * mn * (mn - 1));			//  Variance of runs
	double S  = sqrt(VR);								//  Standard deviation
	
		//  compute area under the normal curve with mean ER and standard deviation S between -runs and runs
	double D = fabs(runs - ER);							//  difference between observed runs and expected runs
	double P = 1.0 - statistics_cmnorm_tt(ER - D, ER + D, ER, S);			//  p-value is area outside the observed number of runs
	
		//  present analysis
	printf("\n\n\n    H0: The number of runs indicates randomness.\n");
	printf("    HA: The number of runs indicates non-randomness.\n\n");
	printf("    Alpha: %4.2f\n\n", ALPHA);
	
	printf("    Positive values:               %4u\n", ones);
	printf("    Negative values:               %4u\n", SEQUENCE - ones);
	printf("    Observed number of runs:       %4u\n", runs);
	printf("    Expected number of runs:       %7.2f\n", ER);
	printf("    Standard deviation:            %7.2f\n", S);
	printf("    P-value:                       %7.2f\n", P);	
	printf("    Reject H0 (P < Alpha)?         %4s\n\n\n", (P < ALPHA) ? "YES" : "NO");
	
	return 0;
}