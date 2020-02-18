#include "fun1.h"

#include <cmath>

namespace calc {

class Ln : public Function1 {
public:
  double call(double a) { return log(a); }
};

}
