// Task9 for computer architecture subject
// Calculate quadratic equation
// by Mark Vodyanitskiy, Elena Bova, Danil Maltsev
#include <stdio.h>
extern double root1;
extern double root2;
extern double a, b, c, d;

int main() {
  while (1) {
    printf("\nEnter coefficients a, b, c: ");
    scanf("%lf%lf%lf", &a, &b, &c);
    sq_eq();
    printf("Discriminant %lf\n", d);
    if (d > 0.0)
      printf("First root = %lf \nSecond root = %lf\n", root1, root2);
    else if (d == 0.0)
      printf("Root = %lf\n", root1);
  }
}