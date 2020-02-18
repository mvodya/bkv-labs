// Software engineering
// Lab 5

#include <string>

#include "stackmap.h"
#include "parser.tab.cc"

namespace calc {

double calculate(std::string input) {
  while(!yy::sequence.empty()) { yy::sequence.pop(); }
  error_handler::error = 0;
  yy::berror = 0;
  yy::sequencer(input);
  yy::parser parse;
  parse();
  
  if (yy::berror) {
    error_handler::set(yy::error_msg);
    return -1;
  }

  return yy::result;
}

}