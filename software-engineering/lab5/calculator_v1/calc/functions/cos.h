#include "fun1.h"

#include <cmath>

namespace calc {

class Cos : public Function1 {
public:
  double call(double a) { return cos(a); }
};

}