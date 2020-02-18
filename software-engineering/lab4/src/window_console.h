// Software engineering
// Lab 4
// @mvodya 14/10/19
#pragma once
#include "config.h"

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Text_Display.H>
#include <chrono>

#include "simulator.h"
#include "window_view.h"

typedef std::chrono::high_resolution_clock tclock;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> ttime;

namespace UI {

class Console : public Fl_Double_Window {
  /* UI groups*/
  Fl_Group* ui_groupGeneralSettings;
  Fl_Group* ui_groupRabbitPrefs;
  Fl_Group* ui_groupFoxPrefs;
  Fl_Group* ui_groupGrassPrefs;

  /* Group: General settings */
  Fl_Spinner* ui_generalSizeX;
  Fl_Spinner* ui_generalSizeY;
#ifndef LESS_UI_MODE
  Fl_Counter* ui_generalTickRate;
#endif  // LESS_UI_MODE
  Fl_Button* ui_generalResetDefault;

  /* Group: Rabbit prefs */
  Fl_Spinner* ui_rabbitGrow;
  Fl_Spinner* ui_rabbitMigration;
  Fl_Spinner* ui_rabbitEat;
  Fl_Spinner* ui_rabbitDeath;

  /* Group: Fox prefs */
  Fl_Spinner* ui_foxGrow;
  Fl_Spinner* ui_foxMigration;
  Fl_Spinner* ui_foxEat;
  Fl_Spinner* ui_foxDeath;

  /* Group: Grass prefs */
  Fl_Spinner* ui_grassGrow;
  Fl_Spinner* ui_grassMigration;

  /* Control buttons */
  Fl_Button* buttonOpenView;
  Fl_Button* buttonPlay;
  Fl_Button* buttonReset;

  /* Output info's */
#ifndef LESS_UI_MODE
  Fl_Output* outCurrentTick;
  Fl_Output* outTickRate;
  Fl_Output* outStatus;
#endif  // LESS_UI_MODE

  /* Events */
  static void evt_OpenViewWindow(Fl_Button* b, Console* v);
  static void evt_SizesChanges(Fl_Spinner* s, Console* v);
  static void evt_WorldReset(Fl_Button* b, Console* v);
  static void evt_Run(Fl_Button* b, Console* v);
  static void evt_UpdateMulti(Fl_Spinner* s, Console* v);
  static void evt_ResetToDefault(Fl_Button* b, Console* v);

  bool isViewOpened;
  bool isSimulationRun;
  UI::View* windowView;

  // Ticks per second counter
  ttime tick_tack;
  unsigned int tacks;

  /* Threads */
  static void tickThread(Console* v);

 public:
  Console();
  Simulator* world;

  /* Public Events */
  static void evt_CloseViewWindow(Console* v);

  void worldReset();
  void reloadParams();
  void setDefault();
};

}  // namespace UI