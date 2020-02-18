#include "fun1.h"

#include <cmath>

namespace calc {

class Lg : public Function1 {
public:
  double call(double a) { return log10(a); }
};

}
