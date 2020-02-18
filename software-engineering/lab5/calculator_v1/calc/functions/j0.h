#include "fun1.h"

#include <cmath>

namespace calc {

class J0 : public Function1 {
public:
  double call(double a) { return j0(a); }
};

}
