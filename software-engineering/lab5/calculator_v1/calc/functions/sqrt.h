#include "fun1.h"

#include <cmath>

namespace calc {

class Sqrt : public Function1 {
public:
  double call(double a) { return sqrt(a); }
};

}
