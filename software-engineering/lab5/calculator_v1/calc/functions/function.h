#pragma once
#include "../symbol.h"

namespace calc {

class Function : public Symbol {
public:
  Function(){};
  void set(double v) {}
  bool rewritable() { return false; };
  Type getType() { return S_FUNC; }
};

}