#include "fun2.h"

#include <cmath>

namespace calc {

class Yn : public Function2 {
public:
  double call(double n, double a) { return yn(n, a); }
};

}
