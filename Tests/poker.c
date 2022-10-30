/*
 * poker.c
 *
 * Created: 04. May. 2022
 *  Author: Frank Bjørnø
 *
 * Purpose: 
 *      Play 2598960 poker hands and analyze the frequency of each kind of hand
 *      using a chi square goodness of fit test.
 *
 * Compilation:
 *     From the command line with Microsoft (R) Macro Assembler and 
 *                                Microsoft (R) C/C++ Optimizing Compiler.
 *
 *      1. Assemble rng.asm without linking using the command
 *         ml /c rng.asm
 *      2. Compile and link the program using the command
 *         cl poker.c cards.c statistics.c rng.obj
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


#define CARDS_IN_A_HAND    5

#include "cards.h"
#include "statistics.h"

double chi_test(double *obs, double *exp, int size, char *labels[]);

int main(void)
{		
	randomize();	
	int N = 2598960;
	
	double observed[10] = {0};
	double expected[10] = {0.501177394, 0.422569027, 0.047539015, 0.021128451, 0.003924646, 0.001965401, 0.001440576, 0.000240096, 0.000013851, 0.000001539};
	
	char *labels[11] = {"hand", "High Card", "One Pair", "Two Pair", "Three of a kind", "Straight", "Flush", 
	                    "Full house", "Four of a kind", "Straight flush", "Royal flush"};
	
	Deck *deck = new_deck();	
	Hand *hand = new_hand();
	
		//  play N poker hands and record frequency of each type of hand.
	for (int n = 0; n < N; n++)
	{		
		for (int c = 0; c < CARDS_IN_A_HAND; c++) add_to_hand(deck, hand);		
		observed[hand -> _rating]++;
		
		for (int c = 0; c < CARDS_IN_A_HAND; c++) drop_from_hand(deck, hand, 1);		
				
		sort_deck(deck);
	}
	
		//  prepare the array of expected frequencies.
	for (int c = 0; c < 10; c++) expected[c] *= N;
	
		//  perform chi square goodnes of fit test.
	statistics_csgof(observed, expected, 10, labels, 0.1);	
	
	return 0;
}
