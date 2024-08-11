/*
 *    EE/CSE 474 
 *
 */

/*
 *    Test Code
 */
#include <stdio.h>
#include <math.h>

int nbitsUI();

//    Add your functions at bottom of this file
//
// Function Prototypes:
int n1p(unsigned int x);  // function prototype for required code of 1.1
int n0p(unsigned int x);  // function prototype for required code of 1.2

double pi = 3.14159265358979323846264338327950288419716939937510;

void main() {
  unsigned int examples[4];
  examples[0] =    0b00001100000;   // 1 / 14
  examples[1] =    0b00001110000;   // 1 / 14
  examples[2] = 0b11100110110110;   // 4 / 10
  examples[3] =    0b11111111111;   // 5 / 10
  examples[4] = 0b11111111111111111111111111111111;  // 16  / 0

  printf("\n testing the 1-pair and 0-pair functions:\n");
  for(int i=0;i<5;i++){
      printf("ex: %d   n1p(x): %d\n",i,n1p(examples[i]));
  }
  printf("\n");
    
  for(int i=0;i<5;i++){
      printf("ex: %d   n0p(x): %d\n",i,n0p(examples[i]));
  }
    
  printf("Looked at range: 4,000,000 --> 10,000,000");
  printf("There were %d values (out of 6,000,000) where n0p()+n1p()==16", nbitsUI());
}

int n1p(unsigned int x) {
  int last = 2;
  int curr = 2;
  int ret = 0;
  for(int i = 0; i < 32; i++) {
    curr = x % 2;
    if(last == curr && curr == 1) {
        ret++;
        x >>=1;
    }
    last = x % 2;
    x >>= 1;
  }

  return ret;
}

int n0p(unsigned int x) {
  int last = 2;
  int curr = 2;
  int ret = 0;
    
  for(int i = 0; i < 32; i++) {
    curr = x % 2;
    if(last == curr && curr == 0) {
        ret++;
        x >>=1;
        i++;
    }
    last = x % 2;
    x >>= 1;
  }

  return ret;
}

int nbitsUI() {
    int count = 0;
    for(int i = 4000000; i <= 10000000; i++) {
        if(n0p(i)+n1p(i)==16) {
            count++;
        }
    }

    return count;
}