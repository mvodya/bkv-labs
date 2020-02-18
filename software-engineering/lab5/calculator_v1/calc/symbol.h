// Software engineering
// Lab 5

#pragma once

namespace calc {

class Symbol {
public:
  enum Type { S_FUNC = 0, S_VAR };

public:
  virtual double call() = 0;
  virtual double call(double a) = 0;
  virtual double call(double a, double b) = 0;
  virtual void set(double v) = 0;
  virtual bool rewritable() = 0;
  virtual Type getType() = 0;
};

}