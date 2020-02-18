#include "fun1.h"

#include <cmath>

namespace calc {

class J1 : public Function1 {
public:
  double call(double a) { return j1(a); }
};

}
