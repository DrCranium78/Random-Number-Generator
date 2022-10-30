/*
 * runs_exp.c
 *
 * Created: 06. Sep. 2022
 *  Author: Frank Bjørnø
 *
 * Purpose: 
 *      Count the number of binary runs in every number from 0 to 65635 and store the results in
 *      a datafile to be analyzed in MATLAB(R).
 *
 * Compilation:
 *     From the command line with Microsoft (R) Macro Assembler and 
 *                                Microsoft (R) C/C++ Optimizing Compiler.
 * 
 *      1. Compile and link the program using the command
 *         cl runs_exp.c
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

#define N 65536



/*
 *     This function counts the number of binary runs in a 16 bit number.
 *
 *     \param data     The 16 bit number to be analyzed.
 *
 *     \return         The number of binary runs.
 *
 *     Example:  The binary representation of the number 28024 is 0110 1101 0111 1000.
 *               In binary, evaluated from left to right(lsb to msb) the numnber starts 
 *               with a run of three zeros, followed by a run of four ones. Then three 
 *               single digit runs etc. for a total of nine runs.
 */
uint8_t runs(uint16_t data)
{
	uint8_t bitvalue;
	uint8_t current = 2;				//  initialize to "impossible" value.
	uint8_t runs    = 0;
	
	/*
	 *  Test each bit of data from least significant to most. 
	 *  The loop condition fails when the mask bit is shifted out
	 *  of mask and mask becomes 0.
	 */
	for (uint16_t mask = 0x01; mask > 0x00; mask <<= 1)
	{
		bitvalue = (mask & data) != 0;	//  test bit			
		
		if (bitvalue != current)			//  guaranteed to evaluate to true on the first iteration.
		{
			runs++;
			current = bitvalue;
		}
	}
	return runs;
}



int main(void)
{	
		
	char *format = "%u\n";
	FILE *fp;
	fp = fopen("datafile.dat", "w");
	
		//  count binary runs of every number from 0 to 65535 and store results.
	for (int c = 0; c < N; c++) fprintf(fp, format, runs(c));	
	fclose(fp);
	
	return 0;
}