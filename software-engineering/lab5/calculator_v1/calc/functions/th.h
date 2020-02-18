#include "fun1.h"

#include <cmath>

namespace calc {

class Th : public Function1 {
public:
  double call(double a) { return tanh(a); }
};

}