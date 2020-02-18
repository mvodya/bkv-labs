#pragma once
#include "function.h"
#include "../error_handler.h"

namespace calc {

class Function2 : public Function {
public:
  double call() {
    calc::error_handler::set("Function need argument!");
    return -1;
  }
  
  double call(double a) { 
    calc::error_handler::set("Function with only two arguments!");
    return -1; 
  }
};

}