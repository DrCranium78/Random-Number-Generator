/*
 * acdist.c
 *
 * Created: 13. Jun. 2022
 *  Author: Frank Bjørnø
 *
 * Purpose: 
 *      Draw 10000 samples with sample size 400, compute the autocorrelation lag 1 and store
 *      the results in a datafile to be analyzed in MatLab.
 *
 * Compilation:
 *     From the command line with Microsoft (R) Macro Assembler and 
 *                                Microsoft (R) C/C++ Optimizing Compiler.
 *
 *      1. Assemble rng.asm without linking using the command
 *         ml /c rng.asm
 *      2. Compile and link the program using the command
 *         cl acdist.c rng.obj
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

#define SAMPLESIZE     400
#define    SAMPLES   10000

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
 *            the data sample in *x is drawn from, not the mean of the 
 *            data in x.
 */
double acov(double *x, int size, int lag, double pmean)
{
	double sum = 0.0, *y = x + lag;		
	for (int c = 0; c < size - lag; c++) sum += (*x++ - pmean) * (*y++ - pmean);
	return sum / (size - lag);
}



int main(void)
{
		//  the population mean and variance are known.
	double pop_mean = 0.5;
	double pop_var  = 1.0 / 12.0;
	
	double *data = (double*)malloc(SAMPLESIZE * sizeof(double));
	double *ptr;
	
		//  specify data format and open file for writing
	char *format = "%7.4f\n";
	FILE *fp;
	fp = fopen("datafile.dat", "w");	
	
		//  draw 10000 samples, calculate the autocorrelation lag 1 and store in file.
	for (int c = 0; c < SAMPLES; c++)
	{
		randomize();
		ptr = data;
		for (int c = 0; c < SAMPLESIZE; c++) *ptr++ = rndflt();
		
			//  divide by pop. variance to get autocorrelation.
		fprintf(fp, format, acov(data, SAMPLESIZE, 1, pop_mean) / pop_var);
	}		
	
		//  close file, free memory and end program	
	fclose(fp);
	free(data);
	return 0;
}
