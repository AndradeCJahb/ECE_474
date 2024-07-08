// Filename:    c_prog01.c
// Author:      Christopher Andrade
// Date:        7/12/24
// Description: This .c file contains solutions to multiple exercises to gain experience and proficiency with C programming.

/*c_prog01.c
 *
 *     ECE474 introductory C programming assignment Part1
 *
 *     Student Name: Christopher Andrade
 *     Student ID#:  2221525
 *
 */

/*  Objectives:
       Gain experience and proficiency with C programming.

Format of assignment:
1)  Enter your name into the space above
2)  Read through the comments below and edit code into this file to solve the 
    problems.
3)  Check your results against posted solution
4)  Upload this file only (c_prog01.c) to Canvas

Printing:   Use the following functions to print:
  print_int(int x)                prints out an integer
  print_usi(unsigned int x)       prints out an unsigned int
  print_newl()                    prints a "newline" character
  print_str(char *x)              prints a string (pointed to by x)
  print_dble(double)              prints out a double precision floating point number
*/

#include "c_prog1.h"

/**********************************************************************
                 All variable declarations
**********************************************************************/

int i,j,k,l;      // these make good loop iterators.
int card, suit;

// Part 1.2
int t1, t2;

// Part 1.4
char* st_buffer;

// Part 2.0
shuffle cards[N_DECK][2];

//  Part 2.2
unsigned char testhand[7]={0};

// Part 3.1
char * card_names[] = {"Ace","2","3","4","5","6","7","8","9","10","Jack","Queen",
                        "King"};
char * suit_names[] = {"Hearts","Diamonds","Clubs","Spades"};

// Part 3.2
int dealer_deck_count = 0;  // how many cards have been dealt from a deck

/******************************************************************************
 * 1.1  Write code to print out integers between 1 and N on separate lines
 *      
 *      SAMPLE OUTPUT:
 *       1 
 *       2 
 *       3 
 *       (continued)...
 *****************************************************************************/

// Description: Takes a single integer N, and prints each number from 1 to N on a new line.
void count(int N) {
  for (int i = 1; i <= N; i++) {  // loop through each number from 1 to N
    print_int(i);                 // print current number
    print_newl();                 // print newline to seperate each number
  }
}

/******************************************************************************
 * 1.2  Write code to print out integers between 1 and N on separate lines
 *      
 *      SAMPLE OUTPUT:
 *       1 
 *       2 
 *       3 
 *       (continued)...
 *       [sum(1,2,3 ... N)] [sum(1^2,2^2,3^2 ... N^2)]
 *****************************************************************************/

// Description: Takes a single integer N, prints each number from 1 to N on a new line. Additionally,
//              the sum of every counted number and the sum of their squares are printed at the end.
void sums_and_squares1 (int N) {   
  t1 = 0 ; t2 = 0;                // initialize sum and sum of squares to 0

  for (int i = 1; i <= N; i++) {  // loop through each number from 1 to N
    print_int(i);                 // print current number
    print_newl();                 // print newline to seperate each number
    t1 = t1 + i;                  // increment sum by current number
    t2 = t2 + i * i;              // increment sum of squares by square of current number
  }

  print_int(t1) ;  print_int(t2) ;   print_newl();  // print sums in correct format
}

/******************************************************************************
 * 1.3  Write code to print out integers between 1 and N on separate lines
 *      
 *      SAMPLE OUTPUT:
 *      sum: [sum(1,2,3 ... N)] 
 *      sum of squares: [sum(1^2,2^2,3^2 ... N^2)] 
 *****************************************************************************/

// Description: Takes a single integer N, prints each number from 1 to N on a new line. Additionally,
//              the sum of every counted number and the sum of their squares are printed at the end 
//              in an appropriate format.
void sums_and_squares2 (int N) {
  char *l1 = "sum: ";             // pointers to strings for correct print format
  char *l2 = "sum of squares: ";  
  t1 = 0 ; t2 = 0;                // initialize sum and sum of squares to 0

  for (int i = 1; i <= N; i++) {  // loop through each number from 1 to N
    t1 = t1 + i;                  // increment sum by current number
    t2 = t2 + i * i;              // increment sum of squares by square of current number
  } 

  // print statements ensuring correct print formatting
  print_str(l1);                  // print string concerning sum
  print_int(t1);                  // print sum
  print_newl();                 

  print_str(l2);                  // print string concerning sum of squares
  print_int(t2);                  // print sum of squares
  print_newl();
}

/******************************************************************************
 * 1.4  Write a function, char* length_pad(char *st, char* st_buffer, int n), 
 *      which takes a string and adds the right amount of spaces so that it's 
 *      length is n. It should return a pointer the new string.  If the length 
 *      of st is greater than n, truncate the string. 
 *      Note: Check the variable definitions above.
 * 
 *      Modify 1.3 to use length_pad() so that the numbers are all starting in 
 *      col 21 (i.e. all labels have length 20). 
 *     
 *      SAMPLE OUTPUT:
 *      sum:               [sum(1,2,3 ... N)] 
 *      sum of squares:    [sum(1^2,2^2,3^2 ... N^2)]
 *****************************************************************************/

// Description: Takes a string (st) and integer n to pad and return a pointer to a string (st_buffer) with length n, achieved by
//              padding or truncation of the original string (st).
char* length_pad (char *st, char* st_buffer, int n) {
  st_buffer = (char *)malloc((n+1) * sizeof(char));   // malloc enough to fit newly padded string + null terminator
  int len = strlen(st);

  if (len > n) {                      // word requires truncation case
    for (int i = 0; i < n; i++) {     // copy first n chars from original string to return string
      st_buffer[i] = st[i];
    }
    st_buffer[n] = '\0';              // null terminate the return string

  } else {                            // word requires padding case
    for (int i = 0; i < len; i++) {   // copy all chars from original string to return string
      st_buffer[i] = st[i];
    }
    for (int i = len; i < n; i++) {   // loop to add padding to return string to get n length
      st_buffer[i] = ' ';
    }
    st_buffer[n] = '\0';              // null terminate the return string
  }

  return st_buffer;
}

// Description: Takes a single integer N, prints each number from 1 to N on a new line. Additionally,
//              the sum of every counted number and the sum of their squares are printed at the end 
//              in an appropriate format using padded strings.
void sums_and_squares3 (int N) {
  char *l1 = "sum: ";                 // pointers to strings for correct print format
  char *l2 = "sum of squares: ";      
  t1 = 0 ; t2 = 0;                    // initialize sum and sum of squares to 0

  for (int i = 1; i <= N; i++) {      
    t1 = t1 + i;                      // increment sum 
    t2 = t2 + i * i;                  // increment sum of squares
  }

  // utilize length padding of strings to correctly format results (sums)
  print_str(length_pad(l1,st_buffer, 20)) ; print_int(t1) ; print_newl();
  print_str(length_pad(l2,st_buffer, 20)) ; print_int(t2) ; print_newl();
}

/******************************************************************************
 * 2.1 A 'shuffle' is an array of N_DECK pairs of integers. The first of the 
 *     pair is the card type (0-13 representing ace, 2, 3, .... King) and the 
 *     second representing the suit (hearts, diamonds, clubs, spades).   Thus a 
 *     pair of numbers (1-13)(1-4) describes a unique card in the deck.
 * 
 *     Write a function to fill a shuffle with N_DECK random integer pairs, 
 *     BUT, as with your playing cards, there must be EXACTLY one of each pair 
 *     in the shuffle. Use your function to print out all the "cards" of the 
 *     shuffle, with 7 cards per line.
 * 
 *     To generate a random number use the helper function int randN(int n)
 *     defined at the bottom of this file that returns a random integer between 
 *     1 and N.
 *
 *     SAMPLE OUTPUT:
 [ 11   2 ]  [ 11   4 ]  [ 12   1 ]  [ 5   4 ]  [ 4   3 ]  [ 7   3 ]  [ 5   3 ] 
 [ 13   4 ]  [ 9   3 ]  [ 2   3 ]  [ 1   1 ]  [ 2   4 ]  [ 3   2 ]  [ 2   1 ] 
 [ 13   1 ]  [ 7   4 ]  [ 8   2 ]  [ 4   4 ]  [ 6   4 ]  [ 4   2 ]  [ 1   4 ] 
 [ 7   1 ]  [ 3   3 ]  [ 12   2 ]  [ 6   1 ]  [ 12   4 ]  [ 13   3 ]  [ 9   2 ] 
 [ 9   1 ]  [ 3   4 ]  [ 9   4 ]  [ 10   2 ]  [ 4   1 ]  [ 8   1 ]  [ 1   3 ] 
 [ 3   1 ]  [ 11   3 ]  [ 8   4 ]  [ 10   3 ]  [ 5   1 ]  [ 10   1 ]  [ 13   2 ] 
 [ 11   1 ]  [ 7   2 ]  [ 6   3 ]  [ 8   3 ]  [ 12   3 ]  [ 2   2 ]  [ 5   2 ] 
 [ 1   2 ]  [ 10   4 ]  [ 6   2 ]
 *****************************************************************************/

// Description: Takes a shuffle (array of N_DECK (52) pairs of integers) which is then filled with 52 unique pairs of integers
//              which correspond to valid playing cards with card numbers ranging from 1-13 (Ace to King) and card suits
//              ranging from 1-4 ("Hearts","Diamonds","Clubs","Spades").
void fill (shuffle deck[N_DECK][2]) {
  int cards[N_DECK][2];                       // initialize an "internal" array of cards
  int index = 0;                              // index utilized for populating deck with valid cards

  // nested for-loop to populate "internal" deck with one of each possible and valid card
  for (int card = 1; card <= 13; card++) {
    for (int suit = 1; suit <= 4; suit++) {
      cards[index][0] = card;
      cards[index][1] = suit;
      index++;
    }
  }
  
  // swap each card with a random card up to final card
  for (int i = 0; i < 52; i++) {
    // obtaining random index within cards array
    int j = randN(51);                     

    // temp variables to allow for swapping two cards within cards
    int tempCard = cards[i][0];               
    int tempSuit = cards[i][1];

    // swapping of two cards
    cards[i][0] = cards[j][0];               
    cards[i][1] = cards[j][1];
    cards[j][0] = tempCard;
    cards[j][1] = tempSuit;
  }

  // Copy shuffled cards (cards) to the parameter deck array
  for (int i = 0; i < 52; i++) {
    deck[i][0] = cards[i][0];
    deck[i][1] = cards[i][1];
  }
}

/******************************************************************************
 * 2.2 A 'hand' is an array of seven unsigned chars.  Each char represents one 
 *     card.  We use a four bit field in the char for each of the two numbers 
 *     above: the four most significant bits [7...4] represent the card number 
 *     (1-13) and the lower four [3...0] represent the suit.
 * 
 *     Write functions to:  
 *     a) Convert two integers (from  a shuffle for example) into a char as 
 *        above. If the ints are invalid (e.g. convert(294802984,138142) is not 
 *        a card) return CARD_ERROR
 *     b) Test if a char equals a valid integer pair
 *     c) Get the integer suit from a char
 *     d) Get the integer card from a char
 * 
 *     Both functions a and b must return CARD_ERROR if they get invalid input 
 *     (such as suit > 4).
 * 
 *     Write code for the functions convert(), valid_card(), gcard(), and 
 *     gsuit() below.
 *****************************************************************************/

// Description: Takes two integers, one corresponding to a card number and the other to a card suit, their respective 
//              bit values are concatenated into an unsigned char which is returned. In the case that the card 
//              number and suit are invalid integers i.e. outside of ranges 1-13, 1-4 respectively, CARD_ERROR is returned.
unsigned char convert (int card, int suit) {
  // shift card bits up 4 to allow room for suit bits, OR to combine fields into single unsigned char of 8 bits (1 byte)
  unsigned char converted = ((card << 4) | suit);   

  // Check for valid card using appropriate function, returns converted unsigned char if valid, CARD_ERROR otherwise
  if (valid_card(converted) != CARD_ERROR) {
    return converted;
  }

  return CARD_ERROR;
}

// Description: Takes a single unsigned char and determines if the char is representative of a valid playing card, returns 1
//              if card is valid (represents a card number from 1-13 and a suit from 1-4) or CARD_ERROR is returned if the char
//              does not represent a valid card.
int valid_card(unsigned char card){
  // extract card number and suit using apprpriate functions
  int cardNum = gcard(card);
  int cardSuit = gsuit(card);

  // check both card number and suit for valid integer (1-13 and 1-4 respectively), return 1 if valid, CARD_ERROR if not.
  if (cardNum >= 1 && cardNum <= 13 && cardSuit >= 1 && cardSuit <= 4) {
    return 1; 
  }

  return CARD_ERROR;
}

// Description: Takes an unsigned char and returns card number.
int gcard (unsigned char card) {
  return card >> 4;               // obtain card number by right shifting char 4 times.
}

// Description: Takes an unsigned char and returns card suit.
int gsuit (unsigned char card) {
  return card & 0xF;              // obtain card suit by masking bits of char by 0xF (lower 4 bits remain)
}

/******************************************************************************
 * 3.1 Write a function names(int card, int suit, char answer[]) which places a 
 *     string of the name and suit of a card in the array answer[]. For 
 *     example: name(11,1) → “Jack of Hearts” name(8,2) → “8 of Diamonds”. Use 
 *     the arrays defined above card_names[] and suit_names[]
 *     
 *     Hint: Use pointers to copy the characters one-by-one into the array 
 *     answer[] to build up the final string.
 *****************************************************************************/

// Description: Takes a card and suit integer to create a correctly formatted string which states the card corresponding to the
//              card and suit parameters (ex: Ace of Diamonds). This string is then pointed to by the answer parameter.
void names (int card, int suit, char *answer) {
  // initialize pointers to appropriate strings using card/suit_names arrays based on card/suit
  char* cardStr = card_names[card-1];
  char* suitStr = suit_names[suit-1];

  // obtain length of card and suit strings
  int cardLen = strlen(cardStr);
  int suitLen = strlen(suitStr);

  // copy card string into answer string
  for (int i = 0; i < cardLen; i++) {
    answer[i] = cardStr[i];
  }

  // copy " of " into next indicies of answer string
  answer[cardLen] = ' ';
  answer[cardLen+1] = 'o';
  answer[cardLen+2] = 'f';
  answer[cardLen+3] = ' ';

  // copy suit string into next indicies of answer string
  for (int i = 0; i < suitLen; i++) {
    answer[i + cardLen + 4] = suitStr[i];
  }

  // null terminate the answer string
  answer[cardLen + suitLen + 4] = '\0';
}

/******************************************************************************
 * 3.2 Write a function to deal a hand of M (0<M<8) cards from a shuffle. Use a 
 *     global variable int dealer deck count to keep track of how many cards 
 *     have been dealt from the deck.
 * 
 *     To test this write a function printhand() that prints out a hand of 
 *     cards
 *     
 *     SAMPLE OUTPUT:
 * 
 *     ----testing deal: hand:  0 
 *     Deck count:  0 
 *     --------dealt hand: 
 *     the hand: 
 *         Jack of Diamonds
 *         Jack of Spades
 *         Queen of Hearts
 *         5 of Spades
 *         4 of Clubs
 *         7 of Clubs
 *         5 of Clubs
 * 
 *****************************************************************************/

// Description: Deals M cards from deck[N_DECK][2] parameter into hand[7], places M unsigned chars from array of integer pairs (deck)
//              into hand array, it is also ensured that the same cards are not dealt twice from the same deck.
void deal (int M, unsigned char hand[7], int deck[N_DECK][2]) {
  // for loop to put converted unsigned char of next M cards into hand array
  for (int i = 0; i < M; i++) {
    hand[i] = convert(deck[i+dealer_deck_count][0], deck[i+dealer_deck_count][1]);
  }

  // increment dealer_deck_count by number of cards dealt (M)
  dealer_deck_count = dealer_deck_count + M;
}

// Description: Takes M (number of cards in hand), hand and buffer (buff1) to print the current hand (hand).
void printhand (int M, unsigned char* hand, char* buff1) {
  // print initial hand formatting
  print_str("the hand:");
  print_newl();

  // for loop to print each card in a dealt hand, use names() function to 
  // format unsigned char into readable string ex: "2 of hearts"
  for (int i = 0; i < M; i++) {
    unsigned char card = hand[i];                  // obtain unsigned char from hand
    names(gcard(card), gsuit(card), buff1);        // obtain card and suit and convert to string using names()
    print_str(buff1);                              // print correctly formatted string
    print_newl();
  }
}

/******************************************************************************
 * 3.3 Write functions to identify pairs of cards in a hand (two cards with the
 *     same number), three-of-a-kind (three cards with the same number), and 
 *     four-of-a-kind (four cards with the same number).
 *****************************************************************************/

// Description: takes hand and its size (M) to determine if any pairs of card numbers exist in the hand.
int pairs (int M, unsigned char hand[]) {
  int seen[7];                      // initialize array of seen card numbers

  // clear seen array upon every method call
  for (int k = 0; k < M; k++) {
    seen[k] = 0;
  }

  int numPairs = 0;                 // int counting number of found pairs
  int seenIndex = 0;                // index for adding newly seen card numbers to seen[]

  // loop through each card within a hand
  for (int i = 0; i < M; i++) {
    // check if current card number within hand has been seen yet
    int contained = 0;              
    int curr = gcard(hand[i]);
    
    for (int j = 0; j < M; j++) {
      if (seen[j] == curr) {
        // if contained, contained is true (1) and clear the card number from the seen array
        contained = 1;
        seen[j] = 0;
      }
    }
    
    // increment num pairs if card number has been seen already
    if (contained) {
      numPairs++;
    } else {  // if not seen copy card number to seenIndex and increment seenIndex
      seen[seenIndex] = curr;
      seenIndex++;
    }
  }
  return numPairs;
}

// Description: takes hand and its size (M) to determine if any three-of-a-kinds (of card numbers) exist in the hand.
int trip_s (int M, unsigned char hand[]) {
  int tripsCount = 0;
  unsigned char seenTrip = 0;         // Array to store seen three-of-a-kinds to ensure correct returned count.

  // triple-nested for loops to search every possible combination of 3 cards for a three-of-a-kind.
  for (int i = 0; i < M-2; i++) {
    for (int j = i+1; j < M-1; j++) {
      for (int k = j+1; k < M; k++) {
        if (gcard(hand[i]) == gcard(hand[j]) &&
            gcard(hand[j]) == gcard(hand[k]) && 
            gcard(hand[i]) != seenTrip ) {        // Check if card numbers of current 3 cards match (and not previously seen)
          seenTrip = hand[i];                     // Mark current found Triple
          tripsCount++;                           // increment tripsCount 
        }
      }
    }
  }

  return tripsCount;
}

// Description: takes hand and its size (M) to determine if any four-of-a-kinds (of card numbers) exist in the hand.
int four_kind (int M, unsigned char hand[]) {
  // quadruple-nested for loops to search ePvery possible combination of 4 cards for a four-of-a-kind.
  for (int i = 0; i < M-3; i++) {
    for (int j = i+1; j < M-2; j++) {
      for (int k = j+1; k < M-1; k++) {
        for (int l = k+1; l < M; l++) { 
          if (gcard(hand[i]) == gcard(hand[j]) &&
              gcard(hand[j]) == gcard(hand[k]) &&
              gcard(hand[k]) == gcard(hand[l])){   // Check if card numbers of current 4 cards match
            return 1;     // At most a single four-of-a-kind in a hand of 7 cards, therefore 1 is returned.
          }
        }
      }
    }
  }

  return 0;
}

// Description: Given, predefined helper function to return a random integer between 1 and n.
int randN (int n) { 
    double x;
    x = 1.0 + (double) n * rand() / RAND_MAX;
    return((int)x);
}