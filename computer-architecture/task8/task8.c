// Task8 for computer architecture subject
// Hofstadter sequence
// by Mark Vodyanitskiy, Elena Bova

#include "stdio.h"
unsigned long long int digits[3000040];

int main() {
  digits[0] = digits[1] = 1;

  qnumbers();

  int n = 1;
  // с 3 000 000 по 3 000 020 (20 цифр)
  for (int i = 3000000; i < 3000020; i++) {
    printf("[%2d] %5lld\n", n, digits[i]);
    n++;
  }
  return 0;
}
