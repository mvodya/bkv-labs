// Software engineering
// Lab 4
// @mvodya 14/10/19
#pragma once
#include "config.h"

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Color_Chooser.H>

#include "simulator.h"

namespace UI {

class View : public Fl_Double_Window {
  int cell_size;

  // Console window
  void *windowConsole;
  // Simulator
  Simulator *simulator;

  int size_x, size_y;
  Fl_Button **buttons;

  /* Events */
  static void evt_closeWindow(Fl_Double_Window *w, View *v);
  static void evt_clickButton(Fl_Button *b, View *v);

 public:
  View(void *con);
  ~View();

  void worldUpdate();
};

}  // namespace UI