#include "stdio.h"

  // с 3 000 000 по 3 000 020 (20 цифр)
unsigned long long int digits[3000040];

int main() {
  
  digits[0] = digits[1] = 1;

  qnumbers();

  int n = 1;
  for(int i = 3000000; i < 3000020; i++) {
    printf("[%2d] %5lld\n", n, digits[i]);
    n++;
  }
  return 0;
}
