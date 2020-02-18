// Software engineering
// Lab 5

#include <iostream>
#include "calc.h"

int main() {
  string input;
  cout.precision(8);
  while(1) {
    getline(std::cin, input);
    if (input == "exit" || input == "quit" || input == "q") return 0;
    if (input == "help" || input == "h") {
      cout << HELP_DATA;
      continue;
    }
    double r = calc::calculate(input);
    if (calc::error_handler::error) {
      std::cout << "Error: " << calc::error_handler::error_message << std::endl;
      continue;
    }
    double r_rounded = (int)(r * 100000.0) / 100000.0;
    cout << "Result: " << r_rounded << "\n";
  }
}