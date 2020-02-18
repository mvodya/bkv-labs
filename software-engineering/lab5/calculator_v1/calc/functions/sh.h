#include "fun1.h"

#include <cmath>

namespace calc {

class Sh : public Function1 {
public:
  double call(double a) { return sinh(a); }
};

}
