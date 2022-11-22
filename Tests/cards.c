/*
 * cards.c
 *
 * Created: 04. May. 2022
 *  Author: Frank Bjørnø
 *
 * Purpose: 
 *      Organize a deck of cards and poker hands
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
#include <stdlib.h>

#include "cards.h"


#define ONE_PAIR             0x01
#define TWO_PAIRS            0x02
#define THREE_OF_A_KIND      0x04
#define FOUR_OF_A_KIND       0x08
#define STRAIGHT             0x10
#define FLUSH                0x20
#define ROYAL                0x40


	//  function defined in rng.asm
extern  int rndint(int min, int max);


/***************************************************************************************************
 *                                                                                                 *
 *                                             Card                                                *
 *                                                                                                 *
 ***************************************************************************************************/

/*
 *     sets the _string member of a Card struct
 *
 *     \param *c        Pointer to a Card struct.
 */
void init_string(Card *c)
{
	char *string = c -> _string;
	
	switch(c -> _suit)
	{
		case 0:  *string++ = 5; break;		//  clubs
		case 1:  *string++ = 4; break;		//  diamond
		case 2:  *string++ = 3; break;		//  hearts
		case 3:  *string++ = 6; break;		//  spades
		default: break;				//  impossible
	}
	
	switch(c -> _value)
	{
		case 14: *string++ = 'A'; break;
		case 13: *string++ = 'K'; break;
		case 12: *string++ = 'Q'; break;
		case 11: *string++ = 'J'; break;
		case 10: sprintf(string, "%d", c -> _value); string += 2; break;
		default: sprintf(string, "%d", c -> _value); string++; break;
	}
	*string = 0;
}


/*
 *     Creates a new card and returns a pointer to it
 *
 *     \param rating     A number between 0 and 51 representing a 
 *                       card where 0 is the two of clubs and 51 is
 *                       the ace of spaces.
 */
Card *new_card(unsigned int rating)
{
	Card *c = (Card *)malloc(sizeof(Card));
	if (rating > 51) rating = 51;
	c -> _rating = rating;
	c -> _value  = rating / 4 + 2;
	c -> _suit   = rating % 4;
	init_string(c);
	return c;
}


/*
 *     Compares two cards. Only called from qsort
 */
int cmp_card(const void *a, const void *b)
{		
	return (**(Card**)a)._rating - (**(Card**)b)._rating;		
}



/***************************************************************************************************
 *                                                                                                 *
 *                                             Deck                                                *
 *                                                                                                 *
 ***************************************************************************************************/
 
/*
 *     Creates a new deck of cards and returns a pointer to it
 */
Deck *new_deck()
{
	Deck *d = (Deck *)malloc(sizeof(Deck));
	for (int c = 0; c < 52; c++) d -> _cards[c] = new_card(c);
	d -> _size = 52;	
	return d;
}


/*
 *     Print a deck to the console.
 *
 *     \param *d     A pointer to the deck to be printed.
 *
 *     Note:    This function is never called from the poker program.
 */ 
void print_deck(Deck *d)
{
		//  if deck is empty, print nothing
	if (d -> _size == 0) return;
	
	for (int i = 0; i < d -> _size;)
	{
		printf("%-3s", d -> _cards[i] -> _string);
		printf("%s", (++i % 13 == 0 ? "\n" : " "));
	}
}


/*
 *     Deal a card from the deck
 *
 *     \param *d    The deck from which to deal
 *     
 *     \return      A pointer to the card that was dealt.
 *
 *     Note:   Cards are not dealt from the top of the deck but drawn at random
 *             from the array of cards. This is because the deck is not shuffled.
 */
Card *deal_from_deck(Deck *d)
{
		//  if deck is empty, return NULL.
	if (d -> _size == 0) return NULL;
	
		//  select random card from the _cards array
	int index = rndint(0, --(d -> _size));	
	Card *c = d -> _cards[index];
		
		//  move all cards after the index up in the array
	for (int i = index; i < d -> _size; i++) d -> _cards[i] = d -> _cards[i + 1];
	
	return c;
}


/*
 *     Return a card to the deck
 *
 *     \param *d    Pointer to the deck to which the card is returned.
 *     \param *c    Pointer to the card that is returned.
 *
 *     \return      0 if card is not accepted, 1 if it is.
 *
 *     Note:    It is advisable to not operate with more than one deck.
 */
int return_to_deck(Deck *d, Card *c)
{
		//  don't accept a card in a full deck
	if (d -> _size == 52) return 0;
	
		//  check if the card is already in the stack
	int index;
	for (index = 0; d -> _cards[index] -> _rating != c -> _rating && index < d -> _size; index++);
	
		//  no duplicates.
	if (index < d -> _size) return 0;
	
		//  insert the card at the bottom of the deck.
	d -> _cards[(d -> _size)++] = c;	
	return 1;
}


/*
 *     Sort a deck
 *
 *     \param *d    Pointer to the deck to be sorted.
 *
 *     Note:    The cards are sorted in the order c2, d2, h2, s2, c3, ... , hA, sA. 
 */
void sort_deck(Deck *d)
{
	qsort(d -> _cards, d -> _size, sizeof(Card*), cmp_card);
}


/***************************************************************************************************
 *                                                                                                 *
 *                                             Hand                                                *
 *                                                                                                 *
 ***************************************************************************************************/
 
/*
 *     Creates a new hand and returns a pointer to it.
 */
Hand *new_hand()
{	
	Hand *h = (Hand *)calloc(1, sizeof(Hand));	
	strcpy(h -> _description, "UNRATED");	
	
	return h;
}

/*
 *     Get the number of cards in a given hand.
 */
int hand_size(Hand *h)
{
	return h -> _size;
}


/*
 *     Print a hand to the console, e.g. "♣3 ♥5 ♦8 ♥J ♠J"
 *
 *     \param *d     A pointer to the hand to be printed.
 *
 *     Note:    This function is never called from the poker program.
 */ 
void print_hand(Hand *h)
{
	if (h -> _size == 0) return;
	
	for (int i = 0; i < h -> _size; i++) printf("%s%s", h -> _cards[i] -> _string, (i == h -> _size - 1 ? "\n" : " "));
	printf("%s\n", h -> _description);
}


/*
 *     Sort a deck
 *
 *     \param *d    Pointer to the deck to be sorted.
 *
 *     Note:    The cards are sorted from low to high card
 */
void sort_hand(Hand *h)
{		
	qsort(h -> _cards, h -> _size, sizeof(Card*), cmp_card);		
}


/*
 *     Calculate a score for a given poker hand
 *
 *     \param *h    Pointer to the hand to score.
 *
 *     \return      A score from 0 to 9
 *
 *     Note:    A score from 0 (No Pair/High Card) to 9 (Royal Flush) is assigned
 *              to the hand. This score is used in the main program to categorize
 *              poker hands into ten categories. The score could also be used to
 *              compare poker hands, but there is not need for this in a poker test.
 */
int rate_hand(Hand *h)
{	
		//  a hand with less than 5 cards has no rating.
	if (h -> _size < 5) return 0;

		//  default rating
	h -> _rating = 0;
	strcpy(h -> _description, "HIGH CARD");	
	
		//  make 13 bins and sort cards into bins by value (2, 3, ..., K, A)
	int values[13] = {0}, i;
	for (i = 0; i < h -> _size; i++) values[h -> _cards[i] -> _value - 2]++;
	
		//  look through the bins, count pairs, three and four of a kind
	int pairs = 0, three = 0, four = 0;
	for (i = 0; i < 13; i++)
	{
		switch(values[i])
		{
			case 2:  pairs++; break;
			case 3:  three++; break;
			case 4:  four++;  break;
			default:          break;			//  if 0 or 1, do nothing
		}
	}
	
		//  use 8 bits to keep track of properties (one pair, three of a kind, flush, straight etc.)
	unsigned char properties = 0;
	properties |= (pairs == 1) * ONE_PAIR | (pairs == 2) * TWO_PAIRS | three * THREE_OF_A_KIND | four * FOUR_OF_A_KIND;
	
	if (properties)
	{
		switch(properties)
		{
			case 0x01:
				h -> _rating = 1;				
				strcpy(h -> _description, "ONE PAIR");
				break;
			case 0x02:
				h -> _rating = 2;				
				strcpy(h -> _description, "TWO PAIRS");
				break;
			case 0x04:
				h -> _rating = 3;				
				strcpy(h -> _description, "THREE OF A KIND");
				break;
			case 0x05:
				h -> _rating = 6;				
				strcpy(h -> _description, "FULL HOUSE");
				break;
			case 0x08:
				h -> _rating = 7;				
				strcpy(h -> _description, "FOUR OF A KIND");
				break;
			default: break;								//  impossible if properties != 0
		}		
				//  more than one of  a kind rules out straight and flush, so return early
		return h -> _rating;
	}
	
		//  look for straights. flags uses 13 bits to keep track of different card values.
		//  f.ex. c3, h5, s5, sJ, dQ = 0 0110 0000 1010 while s3, h4, s5, d6, c7 = 0 0000 0011 1110
		//  five 1's in a row indicates a straight
	unsigned short int flags = 0, flag = 1;
	for (i = 0; i < h -> _size; i++) flags |= flag << h -> _cards[i] -> _value - 2;
	
		//  look for the straight bit pattern. 0x1f is the pattern 1 1111. 	
	for (flag = 0x1f; flags != flag && flag <= 0x3e00; flag <<= 1);
	
		//  2, 3, 4, 5, A (0x100f) also counts as a straight. look for this pattern
		//  at this point, properties = 0 (no more than one of a kind)
	properties |= STRAIGHT * ((flag <= 0x1f00) | (flags == 0x100f));
	
		//  look for a flush. the criterion is that all cards have the same suit as the first	
	for (i = 1; i < 5 && (h -> _cards[i] -> _suit) == (h -> _cards[0] -> _suit); i++);	
	properties |= FLUSH * (i == 5);
	
		//  if flush, check for royal straight	
	properties |= ROYAL * ((i == 5) && (flags == 0x1f00));
	
	if (properties)
	{
		switch(properties)
		{
			case 0x10:
				h -> _rating = 4;				
				strcpy(h -> _description, "STRAIGHT");
				break;
			case 0x20:
				h -> _rating = 5;				
				strcpy(h -> _description, "FLUSH");
				break;
			case 0x30:
				h -> _rating = 8;				
				strcpy(h -> _description, "STRAIGHT FLUSH");				
				break;
			case 0x70:									//  STRAIGHT | FLUSH | ROYAL
				h -> _rating = 9;				
				strcpy(h -> _description, "ROYAL FLUSH");
				break;			
			default:				
				break;
		}
	}	
	return h -> _rating;
}


/*
 *     Add a card to a hand.
 *
 *     \param *d    Pointer to Deck to deal from.
 *     \param *h    Pointer to hand to deal to.
 *
 *     \return      Number of cards in hand after adding the card.
 */
int add_to_hand(Deck *d, Hand *h)
{
	if (h -> _size == 5) return 5;
	
	h -> _cards[(h -> _size)++] = deal_from_deck(d);
		
		//  if hand has 5 cards, sort and evaluate
	if (h -> _size == 5)
	{		
		sort_hand(h);		
		rate_hand(h);
	}
	
	return h -> _size;
}


/*
 *     Drop card from hand and return it to a deck.
 *
 *     \param *d    Pointer to deck to return card to.
 *     \param *h    Pointer to hand to drop card from.
 *     \param  i    Index of the card to drop (1 - 5)
 */
int drop_from_hand(Deck *d, Hand *h, unsigned int i)
{	
		//  make sure input is well behaved and that hand is not empty
	if (--i > h -> _size || h -> _size == 0) return h -> _size;
	
	return_to_deck(d, h -> _cards[i]);
	
		//  reset rating. A hand with less than 5 cards has no rating
	h -> _rating = 0;
	strcpy(h -> _description, "UNRATED");
	
		//  if the index is the last card, then just set the index to NULL
	if (i == h -> _size)
	{
		h -> _cards[i] = NULL;		
		return --(h -> _size);
	}
	
		//  if not, cards must be moved within the _cards array
	for (int c = i; c < h -> _size - 1; c++) h -> _cards[c] = h -> _cards[c + 1];
	h -> _cards[--(h -> _size)] = NULL;
		
	return h -> _size;
}
