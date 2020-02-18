// Software engineering
// Lab 4
// @mvodya 14/10/19
#pragma once

#include "config.h"

#include "FL/Fl.H"

class Voxel {
  double grass_, rabbit_, fox_;

 public:
  Voxel(double grass = .0f, double rabbit = .0f, double fox = .0f)
      : grass_(grass), rabbit_(rabbit), fox_(fox) {}

  // All functions have clamp (0.0 ... 1.0)
  // + remove very little values (above 0.000001)
  void grass(double v) {
    grass_ = (v < .0f) ? .0f : (v > 1.f) ? 1.f : v;
    if (grass_ < LOW_VALUE_REMOVE) grass_ = 0;
  }
  void rabbit(double v) {
    rabbit_ = (v < .0f) ? .0f : (v > 1.f) ? 1.f : v;
    if (rabbit_ < LOW_VALUE_REMOVE) rabbit_ = 0;
  }
  void fox(double v) {
    fox_ = (v < .0f) ? .0f : (v > 1.f) ? 1.f : v;
    if (fox_ < LOW_VALUE_REMOVE) fox_ = 0;
  }

  double grass() { return grass_; }
  double rabbit() { return rabbit_; }
  double fox() { return fox_; }

  inline void grassAdd(double v) {
    grass_ += v;
    grass(grass_);
  }
  inline void grassRemove(double v) {
    grass_ -= v;
    grass(grass_);
  }

  inline void rabbitAdd(double v) {
    rabbit_ += v;
    rabbit(rabbit_);
  }
  inline void rabbitRemove(double v) {
    rabbit_ -= v;
    rabbit(rabbit_);
  }

  inline void foxAdd(double v) {
    fox_ += v;
    fox(fox_);
  }
  inline void foxRemove(double v) {
    fox_ -= v;
    fox(fox_);
  }

  // Conversion to Fl_Color
  operator Fl_Color() const {
    Fl_Color color = fl_rgb_color(fox_ * 255, grass_ * 255, rabbit_ * 255);
    return color;
  }
};