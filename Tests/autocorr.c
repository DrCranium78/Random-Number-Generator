/*
 * autocorr.c
 *
 * Created: 24. Jun. 2022
 *  Author: Frank Bjørnø
 *
 * Purpose: 
 *      To test for autocorrelation in a series of numbers generated by the random number
 *      generator in rng.asm.
 *
 * Compilation:
 *     From the command line with Microsoft (R) Macro Assembler and 
 *                                Microsoft (R) C/C++ Optimizing Compiler.
 *
 *      1. Assemble rng.asm without linking using the command
 *         ml /c rng.asm
 *      2. Compile and link the program using the command
 *         cl autocorr.c statistics.c rng.obj
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
#include <math.h>

#include "statistics.h"

#define SAMPLESIZE  400
#define     MAXLAG   20

/*
 *     For a two tailed test, an alpha of 0.05 corresponds to a z score of 1.96
 */
#define ALPHA       0.05
#define ZSCORE      1.96

/*
 *     Procedures defines in rng.asm
 */
extern   void randomize(void);
extern double rndflt(void);


/*
 *     A function to calculate the autocovariance with a specific lag
 *     and with known population mean. This functions is designed 
 *     specifically for the problem at hand.
 *
 *     \param *x      pointer to array with data
 *     \param  size   number of elements in the data array
 *     \param  lag    
 *     \param  pmean  population mean of the data in the data array
 *
 *     note:  size means number of elements in the data array, not
 *            number of pairs of xn and xn+lag
 *
 *     note:  population mean is the mean of the population from which
 *            the data sample in *x is drawn from, not necessarily the 
 *            mean of *x.
 */
double _acov(double *x, int size, int lag, double pmean)
{
	double sum = 0.0, *y = x + lag;		
	for (int c = 0; c < size - lag; c++) sum += (*x++ - pmean) * (*y++ - pmean);
	return sum / (size - lag);
}



int main(void)
{
	randomize();
	
		//  population parameters are known
	double pop_mean = 0.5;
	double pop_var  = 1.0 / 12;
	double pop_corr = 0.0;
	double pop_serr = 1.0 / sqrt(SAMPLESIZE);
	
		//  allocate memory for buffers
	double         *data = (double*)malloc(SAMPLESIZE * sizeof(double));
	double *correlations = (double*)malloc(    MAXLAG * sizeof(double));
	double *ptr;
	
		//  Generate numbers in the interval [0, 1]. The numbers from rndflt() are raw random numbers divided by rndmax.
	ptr = data;
	for(int c = 0; c < SAMPLESIZE; c++) *ptr++ = rndflt();
	
		//  compute correlations
	ptr = correlations;
	for(int lag = 1; lag <= MAXLAG; lag++) *ptr++ = _acov(data, SAMPLESIZE, lag, pop_mean) / pop_var;
	
		//  analyze correlations
	double serr;					//  standard error of the autocorrelation for the sample
	double width;			     		//  width of confidence interval around autocorrelation coefficient
	double p;					//  p-value = 1.0 - Probability of obtaining the correlation given population parameters	
	   int lag;					
	
		//  print header
	puts("\n\n");
	puts("          Test for autocorrelation in a data set.\n");
	printf("Alpha:     %4.2f\n", ALPHA);
	printf("%3s   %5s      %6s      %6s     %8s     %19s    %9s\n", "Lag", "n", "Coeff.", "P", "Std.Err.", "[95% conf. interv]", "Reject H0");          
	puts("-------------------------------------------------------------------------------------");
	
		//  print analysis
	ptr = correlations;
	for(int c = 0; c < MAXLAG; c++)
	{		
			//  calculate sample parameters
		lag   = c + 1;		
		p     = 1 - statistics_cmnorm_tt(-*ptr, *ptr, pop_corr, pop_serr);
		serr  = 1 / sqrt(SAMPLESIZE - lag - 1);
		width = ZSCORE * serr;
		
			//  print analysis
		printf("%3i   %5i     %7.4f      %6.4f     %7.4f       [  %5.2f, %5.2f  ]", lag, SAMPLESIZE - lag, *ptr, p, serr,  *ptr - width, *ptr + width);
		printf("    %5c\n", (p < ALPHA) ? 'X' : ' ');
		ptr++;
	}
	puts("-------------------------------------------------------------------------------------");
	puts("\n\n\n");
	
		//  free memory and end program
	free(data);
	free(correlations);
	return 0;
}
