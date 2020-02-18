#include "fun1.h"

#include <cmath>

namespace calc {

class Ch : public Function1 {
public:
  double call(double a) { return cosh(a); }
};

}
