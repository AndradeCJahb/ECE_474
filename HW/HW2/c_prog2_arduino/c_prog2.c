// Filename:    c_prog2.c
// Author:      Christopher Andrade
// Date:        7/22/24
// Description: This .c file contains solutions to multiple exercises to gain experience and proficiency with C programming.

/*
 *      ECE 474 HW 2
 *      
 *      Student Name:   Christopher Andrade
 *      Student ID:     2221525
 *
 */

/*  Objectives: Gain experience and proficiency with C programming.

Format of assignment:
  1)  Enter your name and ID# into the space above
  2)  Read through the comments below and edit code into this file and 
      c_prog2.h to solve the problems.
  3)  Test your code by running c_prog2_arduino.ino and comparing the output
      to the posted solution in sample_output.txt

#define constants are NOT required for this assignment but MAY be used.

*/
#include "c_prog2.h"

/*******************************************************************************
   Part 1:  Bitwise operations
*******************************************************************************/
/*
 * Some terminology: SETTING a bit means that if K-th bit is 0, then set it to 1
 * and if it is 1 then leave it unchanged (in other words, a bit is SET if it is
 * 1)
 *
 * CLEARING a bit means that if K-th bit is 1, then clear it to 0 and if it is 0
 * then leave it unchanged (a bit is CLEAR if it is 0)
 *
 * COMPLEMENTING or TOGGLING a bit means that if K-th bit is 1, then change it
 * to 0 and if it is 0 then change it to 1.
*/

/* Part 1.1 Write the function long mangle(long SID) which will take your sudent
 * ID and alter it in ways that depend on the binary representation of it as an
 * integer.
 *
 * Specifications: 
 *     1) right shift the ID by 2 positions 
 *     2) clear bit 6 (counting from the LSB=0) 
 *     3) complement bit 3 (If bit 3 is one, make it 0, If bit 3 is 0, make it 
 *        1)
 *
 *   OUTPUT EXAMPLE:
 *   Part 2.1: Enter your UW Student ID number: 
 *    You entered 51218 
 *    Your mangled SID is 12812
 */

// Description: This function takes a student ID and performs bit manipulation on it.
long mangle(long SID){
    SID >>= 2;
    SID &= ~(1 << 6);
    SID ^= (1 << 3);
    return SID;
}

/* Part 1.2  More bit manipulation. The function bit_check(int data, int bits_on,
 * int bits_off), will check an int to see if a specific mask of  bits is set
 * AND that another bit mask is clear.  Returns 1 if matches and 0 if not.
 * Return -1 for the degenerate case of testing for BOTH off and on for any bit
 *
 * Pseudocode examples of some masks and what they are checking  (Using 4-bit
 * #'s for clarity)
 *
 * bits_on = binary(0011) - this mask means that we are checking if the last
 *                          two LSBs (e.g. 00XX, the X bits) of the data are
 *                          SET (1). bits_off= binary(0100) - this mask means
 *                          that we are checking if the third LSB (e.g. 0X00,
 *                          the X bit) of the data is CLEAR (0).
 *
 * Example of returning -1: bits_on = binary(0110) bits_off= binary(0011) here
 * the second LSB (00X0, the X bit) has to be both SET and CLEAR to pass
 * according to the masks, which is impossible - in this case we return a -1
 * for invalid mask combinations.
 *
 * Again in pseudocode, some examples of actual checks/values that the test
 * code would do: (Using 4-bit #'s for clarity)
 *
 *  int d = binary(1100)  // pseudocode 
 *  int onmask1 =  binary(0011)  
 *  int offmask1 = binary(0100)  
 *  int onmask2 =  binary(0100)
 *  int offmask2 = binary(0001)  
 *
 *  bit_check(d, onmask1, offmask1) --> 0 
 *  bit_check(d, onmask1, offmask2) --> -1 // contradictory
 *  bit_check(d, onmask2, offmask2) --> 1 
 *  bit_check(d, offmask1, offmask1) --> -1 // contradictory
 */

// Description: This function checks if the bits_on and off masks match the inputted data (if all bits_off are 0 and all bits_on are 1).
//              Returns -1 if the bits_on and bits_off masks overlap, 1 if the data matches the masks, and 0 if the data does not match the masks.
int bit_check(int data, int bits_on, int bits_off){
    if (bits_off & bits_on) {
        return -1;
    }

    if(data & bits_on && ~data & bits_off) {
        return 1;
    }

    return 0;
}

/*******************************************************************************
   Part 2:  Pointer declaration and usage
*******************************************************************************/
char a_array[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
    'P','Q','R','S','T','U','V','W','X','Y','Z'};

/* Part 2.1: Write the function char* pmatch(char c)
 * This function will return a pointer to the element in a pre-defined
 * array which matches it's character arg. (accepts capital letters only).
 * If the character cannot be found, return the NULL pointer.
 *
 * Example: pmatch('A') should return a pointer to 'A' in a_array
 * Dereferencing that pointer should then print 'A'
 */

// Description: This function returns a pointer to the inputted character in the a_array.
char* pmatch(char c){
    char* retPtr = NULL;

    for (int i = 0; i < 26; i++) { 
        if(a_array[i] == c) {
            retPtr = &(a_array[i]);
        }
    }

    return retPtr;
}

/* Part 2.2: Write the function char nlet(char* ptr) where ptr is a pointer
 * returned by pmatch. This function will return the next letter of the alphabet
 * (Not a pointer to the next letter of the alphabet) unless the pointer points
 * to 'Z'. In that case it will return -1. If the argument does not point to a
 * capital letter A-Y, return -1.
 */

// Description: This function returns the next letter in the alphabet after the inputted characte.
//              if the inputted character is 'Z' or NULL -1 is returned.
signed char nlet(char *ptr){
    if (*ptr == 'Z' || ptr == NULL) {
        return -1;
    }

    return *(ptr + sizeof(*ptr));
}

/* Part 2.3: Write the function int ldif(char c1, char c2) to find the alphabet
 * distance between two letters using pointers. 
 * Example:     
 *     Ldif('A','E') -> 4
 *
 * If either character is not a capital letter, return a negative number < -26
 */

// Description: This function returns the "alphabet distance" between two inputted characters (c2 - c1).
int ldif(char c1, char c2){
    char* letter1 = pmatch(c1);
    char* letter2 = pmatch(c2);

    if (letter1 && letter2) {
        return (letter2 - letter1) / sizeof(*letter2);
    }

    return -27;
}

/*******************************************************************************
   Part 3: Working with structs
*******************************************************************************/

/* Part 3.1: Define a struct in "c_prog2.h" to represent a custom datatype
 * Person. Modify the existing declaration "typedef struct Person {..." to
 * contain the following:
 *     1) a max 20 char string: FirstName
 *     2) a max 30 char string: LastName
 *     3) a max 80 char string: StreetAddr
 *     4) a max 6 char string: ZipCode 
 *        (a zip code is 5 characters, think why might we want a max of 6?)
 *     5) double: Height      // height in meters
 *     6) float: Weight       // weight in kg
 *     7) long int: DBirth    // birthday (days since 1-Jan-1900)
 *
 * NOTE: The existing declaration shows the general syntax of creating a
 * struct but is just a placeholder with the correct fields so the code
 * will compile and the tests will run.
 */

/* Part 3.2: Write a function personSize that returns the number of bytes
 * required to store the person struct in memory. What do you think the number
 * will be?
 * 
 * HINT: You can use built in library functions for this.
 *
 * NOTE: This should be the same for everyone on the Arduino Mega, but 
 * if you compile for a different chip the size may vary. Look up 
 * 'structure padding in C' to see why. For example, the computer processes 
 * data in the minimum of 4 or 8 bytes for 32/64 bit processors.
 */

// Description: This function returns the size of a person struct in bytes.
int personSize(Person p){
    return sizeof(p);
}
/* Part 3.3: Write a function: per_print(person * p)  which prints out a
 * formatted person. If the person's address is longer than 60 characters,
 * truncate it to 60 characters but do not erase memory of the last 20
 * characters if present.
 * 
 * HINT: You can use functions from the standard string libraries. For example,
 * the sprintf function can help create formatted strings with a mix of words
 * and numbers (see the testing code in c_prog2_arduino.ino for examples). The
 * function strcat can also be useful for concatenating strings together.
 * 
 * Also some microcontrollers do not have hardware support for floating point 
 * operations. You'll notice that Arduino does not support certain standard c
 * functions like the ability to use %f to print floating point numbers with
 * sprintf. Define the helper function floats_to_ints(float f, int* output) that
 * takes in a floating point number and converts it to 2 integers in the int
 * array output. You may find built in math functions such as round helpful.
 *
 * OUTPUT EXAMPLE: 
 *  --- person report: ---- 
 *  First Name:       Blake 
 *  Last Name:        Hannaford 
 *  Address:          124 N. Anystreet / Busytown, WA 
 *  Zip:              99499 
 *  Height:           1.97 
 *  Weight:           81.81 
 *  DOB 1/1/1900:     34780
 *  -----------------------
 */

// Description: This function takes a float and outputs its integer and decimal parts into seperate array indicies within "output".
void float_to_ints(float f, int* output){
    output[0] = (int) f;
    output[1] = (int) ((f - output[0]) * 100);
    return;
}

// Description: This function formats a person struct and places the formatted string into a returned char buffer.
char* per_print(Person *p, char* personbuf) {
    int heightArr[2] = {0, 0};
    int weightArr[2] = {0, 0};

    float_to_ints(p->Height, heightArr);
    float_to_ints(p->Weight, weightArr);
    
    char newAddr[61];
    strncpy(newAddr, p->StreetAddr, 60);
    newAddr[60] = '\0';

    sprintf(personbuf, 
            "--- person report: ----\nFirst Name:       %s\nLast Name:        %s\nAddress:          %s\nZip:              %s\nHeight (m):           %d.%d\nWeight (kg):           %d.%d\nDOB 1/1/1900:     %ld\n", 
            p->FirstName, p->LastName, newAddr, p->ZipCode, heightArr[0], heightArr[1], weightArr[0], weightArr[1], p->DBirth);
    strcat(personbuf, "-----------------------\n");
    return personbuf;
}