// Software engineering
// Lab 4
// @mvodya 14/10/19

// REQUIRE GUI LIBRARY: FLTK 1.3

#include "config.h"

#include <iostream>
#include "window_console.h"

using namespace std;

int main(int argc, char** argv) {
  UI::Console* mainWindow = new UI::Console();
  mainWindow->show(argc, argv);
  return Fl::run();
}