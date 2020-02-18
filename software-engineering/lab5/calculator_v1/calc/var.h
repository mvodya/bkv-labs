// Software engineering
// Lab 5

#pragma once
#include "symbol.h"

#include <cmath>

namespace calc {

class Var : public Symbol {
  double value;
public:
  Var(double v = 0) : value(v) {};
  double call() { return value; }
  double call(double a) { return value; }
  double call(double a, double b) { return value; }
  void set(double v) { value = v; }
  bool rewritable() { return true; };
  Type getType() { return S_VAR; }
};

}