#include "fun1.h"

#include <cmath>

namespace calc {

class Y1 : public Function1 {
public:
  double call(double a) { return y1(a); }
};

}
