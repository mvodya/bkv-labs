#include "fun2.h"

#include <cmath>

namespace calc {

class Jn : public Function2 {
public:
  double call(double n, double a) { return jn(n, a); }
};

}
