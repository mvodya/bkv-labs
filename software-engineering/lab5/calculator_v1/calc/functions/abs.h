#include "fun1.h"

#include <cmath>

namespace calc {

class Abs : public Function1 {
public:
  double call(double a) { return abs(a); }
};

}
