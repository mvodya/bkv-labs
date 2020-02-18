#include "fun1.h"

#include <cmath>

namespace calc {

class Y0 : public Function1 {
public:
  double call(double a) { return y0(a); }
};

}
