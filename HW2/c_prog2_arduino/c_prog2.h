// Filename:    c_prog2.c
// Author:      Christopher Andrade
// Date:        7/12/24
// Description: This .c file contains solutions to multiple exercises to gain experience and proficiency with C programming.

#ifdef __cplusplus
 extern "C" {
#endif
#ifndef C_PROG2_H_INCLUDED
#define C_PROG2_H_INCLUDED

/*
 *      ECE 474 HW 2
 *      
 *      Student Name:   Christopher Andrade
 *      Student ID:     2221525
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*************************************************************************
   Part 2.1:  Bitwise operations
*************************************************************************/
long mangle(long);

int  bit_check(int, int, int);

/*************************************************************************
   Part 2.2:  Basic Pointer declaration and usage
*************************************************************************/
char * pmatch(char);

/*************************************************************************
   Part 2.3:  Pointer Arithmetic
*************************************************************************/
signed char nlet(char*);

int ldif(char, char);

/*************************************************************************
   Part 2.4   struct and sizeof
*************************************************************************/
// Modify this struct definition according to the instructions
// HINT: How long do each of these fields need to be?
typedef struct Person {
    char FirstName[20];
    char LastName[30];
    char StreetAddr[80];
    char ZipCode[6];
    double Height;
    float Weight;
    long int DBirth;
} Person ;

int personSize(Person);

void float_to_ints(float, int*);

// Modify this definition 
// NOTE: This is used to store the output string of per_print.
// How long does this need to be?
char personbuf[sizeof(Person)];

char* per_print(Person*, char*);

#endif // C_PROG2_H_INCLUDED
#ifdef __cplusplus
}
#endif
