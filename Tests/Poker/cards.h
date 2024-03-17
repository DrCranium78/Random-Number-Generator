/*
 * cards.h
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


#pragma once

/*
 *     Card
 */
 
typedef struct card
{
	int  _rating;							//  0 - 51: 0 is the two of clubs and 51 is the ace of spades
	int  _value;							//  2, 3, ...,10, j, Q, K, A
	int  _suit;							//  0 = clubs, 1 = diamonds, 2 = hearts, 3 = spades
	char _string[4];						//  _value and a symbol correpsonding to _suit
	struct card *_next;	
} Card;

Card *new_card(unsigned int rating);					//  Returns a pointer to a new card 
void  destroy_card(Card **c);						//  Free memory associated with this card


/*
 *     Deck
 *     Deck is a linked list with functions for accessing elements
 */
typedef struct
{
	Card *_cards;							//  This is the head of a linked list
	int _size;							//  current number of cards in the deck
} Deck;

Deck *new_deck();							//  Returns a pointer to a new deck
void  destroy_deck(Deck **d);						//  Frees the memory associated with the deck
Card *deal_card(Deck *d);						//  Returns a pointer to a card, removes the card from the deck
 int  return_card(Deck *d, Card *c);					//  Adds a card to the deck


/*
 *     Hand
 */
typedef struct
{
	Card *_cards[5];						//  A hand has max 5 cards
	int   _size;							//  current number of cards in the hand
	int   _rating;							//  0 - 9: 0 is high card, 9 is a royal flush
	char  _description[16];						//  "HIGH CARD", "ONE PAIR", etc
} Hand;

Hand *new_hand();							//  Returns to a new hand
void  destroy_hand(Hand **h);						//  Frees memory associated with the hand. Hand must be empty
 int  add_card(Deck *d, Hand *h);					//  Add a card from a given deck to the hand
 int  drop_card(Deck *d, Hand *h, unsigned int i);			//  Drop a card and return it to a given deck 
void  hand_to_string(Hand *h, char *buffer, int n);			//  Produces a text representation of the hand
