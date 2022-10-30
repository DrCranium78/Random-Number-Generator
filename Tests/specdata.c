/*
 * specdata.c
 *
 * Created: 11. Jun. 2022
 *  Author: Frank Bjørnø
 *
 * Purpose: 
 *      Generate 50000 random 3-coordinate points and store the data in a file.
 *      The numbers are rounded to two decimals. The data will be analyzed in
 *      Matlab.
 *
 * Compilation:
 *     From the command line with Microsoft (R) Macro Assembler and 
 *                                Microsoft (R) C/C++ Optimizing Compiler.
 *
 *      1. Assemble rng.asm without linking using the command
 *         ml /c rng.asm
 *      2. Compile and link the program using the command
 *         cl specdata.c rng.obj
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

extern   void randomize(voif);
extern double rndflt(void);

int main(void)
{
	randomize();
	
	char *format = "%4.2f,%4.2f,%4.2f\n";	
	FILE *datafile;	
	datafile = fopen("rng_data.dat", "w");
	
	for (int n = 0; n < 50000; n++) fprintf(datafile, format, rndflt(), rndflt(), rndflt());							

	fclose(datafile);	
	return 0;
}