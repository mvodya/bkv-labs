#include "fun2.h"

#include <cmath>

namespace calc {

class Log : public Function2 {
public:
  double call(double base, double x) { return log(x) / log(base); }
};

}
