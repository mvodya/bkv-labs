#include "fun1.h"

#include <cmath>

namespace calc {

class Sin : public Function1 {
public:
  double call(double a) { return sin(a); }
};

}
