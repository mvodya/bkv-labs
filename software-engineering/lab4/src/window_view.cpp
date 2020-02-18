// Software engineering
// Lab 4
// @mvodya 14/10/19
#include "window_view.h"
#include "window_console.h"

UI::View::View(void* con)
    : Fl_Double_Window(VIEW_WINDOW_SIZE, VIEW_WINDOW_SIZE, "World view") {
  windowConsole = con;
  UI::Console* console = (UI::Console*)windowConsole;
  simulator = console->world;

  // Set close event
  callback((Fl_Callback*)evt_closeWindow, this);

  // Get values from simulator
  size_x = simulator->size_x;
  size_y = simulator->size_y;

  cell_size = VIEW_WINDOW_SIZE / size_y;
  size(cell_size * size_x, cell_size * size_y);

  // Create buttons
  buttons = new Fl_Button*[size_x * size_y];
  for (int i = 0; i < size_y; i++) {
    for (int j = 0; j < size_x; j++) {
      Fl_Button* b =
          new Fl_Button(j * cell_size, i * cell_size, cell_size, cell_size);
      buttons[(i * size_x) + j] = b;
      b->box(FL_FLAT_BOX);
      b->clear_visible_focus();
      b->callback((Fl_Callback*)evt_clickButton, this);
      b->color(0);
    }
  }
  end();
}

UI::View::~View() {
  delete buttons;
}

void UI::View::worldUpdate() {
  // Update buttons
  for (int i = 0; i < size_y; i++) {
    for (int j = 0; j < size_x; j++) {
      buttons[(i * size_x) + j]->color(simulator->world[i][j]);
    }
  }
  redraw();
}

void UI::View::evt_closeWindow(Fl_Double_Window* w, View* v) {
  // Send close event to console window on view close
  UI::Console::evt_CloseViewWindow((UI::Console*)v->windowConsole);
}

void UI::View::evt_clickButton(Fl_Button* b, View* v) {
  // Get position of clicked button
  int x = b->x() / v->cell_size;
  int y = b->y() / v->cell_size;
  // RGB
  static double red, green, blue;
  // Use last color
  if (Fl::event_alt()) {
    // Set voxel RGB
    v->simulator->world[y][x].fox(red);
    v->simulator->world[y][x].grass(green);
    v->simulator->world[y][x].rabbit(blue);
    // Set button color
    v->buttons[(y * v->size_x) + x]->color(v->simulator->world[y][x]);
    v->redraw();
    return;
  }
  // Get RGB
  red = v->simulator->world[y][x].fox();
  green = v->simulator->world[y][x].grass();
  blue = v->simulator->world[y][x].rabbit();
  // Color picker call
  if (fl_color_chooser("Choose voxel color", red, green, blue)) {
    // Set voxel RGB
    v->simulator->world[y][x].fox(red);
    v->simulator->world[y][x].grass(green);
    v->simulator->world[y][x].rabbit(blue);
    // Set button color
    v->buttons[(y * v->size_x) + x]->color(v->simulator->world[y][x]);
    v->redraw();
  }
}
