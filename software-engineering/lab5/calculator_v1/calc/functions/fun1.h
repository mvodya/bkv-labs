#pragma once
#include "function.h"
#include "../error_handler.h"

namespace calc {

class Function1 : public Function {
public:
  double call() {
    calc::error_handler::set("Function need argument!");
    return -1;
  }
  
  double call(double a, double b) { 
    calc::error_handler::set("Function with only one argument!");
    return -1; 
  }
};

}