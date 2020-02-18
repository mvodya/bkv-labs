#include "fun1.h"

#include <cmath>

namespace calc {

class Tan : public Function1 {
public:
  double call(double a) { return tan(a); }
};

}
