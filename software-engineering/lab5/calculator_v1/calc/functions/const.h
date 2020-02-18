#pragma once
#include "../symbol.h"

#include <cmath>

namespace calc {

class Const : public Symbol {
  const double value;
public:
  Const(double v) : value(v) {};
  double call() { return value; }
  double call(double a) { return value; }
  double call(double a, double b) { return value; }
  void set(double v) {}
  bool rewritable() { return false; };
  Type getType() { return S_VAR; }
};

}