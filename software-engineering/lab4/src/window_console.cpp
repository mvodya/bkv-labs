// Software engineering
// Lab 4
// @mvodya 14/10/19
#include "window_console.h"
#include <string>

UI::Console::Console()
    : Fl_Double_Window(815, 423, "Biocenosis Simulation Console") {
  /* Init general group */
  ui_groupGeneralSettings = new Fl_Group(10, 25, 390, 120, "General settings");
  ui_groupGeneralSettings->box(FL_GTK_UP_FRAME);
  ui_groupGeneralSettings->align(Fl_Align(FL_ALIGN_TOP_LEFT));

  ui_generalResetDefault =
      new Fl_Button(220, 100, 160, 25, "Reset all to default");
  ui_generalResetDefault->callback((Fl_Callback*)evt_ResetToDefault, this);
  ui_generalSizeX = new Fl_Spinner(20, 50, 160, 25, "Size X:");
  ui_generalSizeX->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_generalSizeX->range(3, 200);
  ui_generalSizeX->value(62);
  ui_generalSizeX->callback((Fl_Callback*)evt_SizesChanges, this);
  ui_generalSizeY = new Fl_Spinner(20, 100, 160, 25, "Size Y:");
  ui_generalSizeY->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_generalSizeY->range(3, 200);
  ui_generalSizeY->value(62);
  ui_generalSizeY->callback((Fl_Callback*)evt_SizesChanges, this);
#ifndef LESS_UI_MODE
  ui_generalTickRate = new Fl_Counter(220, 50, 160, 25, "Tick rate / sec:");
  ui_generalTickRate->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_generalTickRate->step(1);
  ui_generalTickRate->lstep(5);
  ui_generalTickRate->value(10.0);
  ui_generalTickRate->range(1.0, 500.0);
#endif  // LESS_UI_MODE

  ui_groupGeneralSettings->end();

  /* Init rabbit prefs group */
  ui_groupRabbitPrefs = new Fl_Group(10, 185, 190, 225, "Rabbit prefs");
  ui_groupRabbitPrefs->box(FL_GTK_UP_FRAME);
  ui_groupRabbitPrefs->align(Fl_Align(FL_ALIGN_TOP_LEFT));

  ui_rabbitGrow = new Fl_Spinner(20, 210, 170, 25, "Grow multiplier:");
  ui_rabbitGrow->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_rabbitGrow->type(FL_FLOAT_INPUT);
  ui_rabbitGrow->range(0.0, 1.0);
  ui_rabbitGrow->step(0.05);
  ui_rabbitGrow->callback((Fl_Callback*)evt_UpdateMulti, this);
  ui_rabbitGrow->value(0.1);
  ui_rabbitMigration =
      new Fl_Spinner(20, 260, 170, 25, "Migration multiplier:");
  ui_rabbitMigration->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_rabbitMigration->type(FL_FLOAT_INPUT);
  ui_rabbitMigration->range(0.0, 1.0);
  ui_rabbitMigration->step(0.05);
  ui_rabbitMigration->callback((Fl_Callback*)evt_UpdateMulti, this);
  ui_rabbitEat = new Fl_Spinner(20, 310, 170, 25, "Eat multiplier:");
  ui_rabbitEat->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_rabbitEat->type(FL_FLOAT_INPUT);
  ui_rabbitEat->range(0.0, 1.0);
  ui_rabbitEat->step(0.05);
  ui_rabbitEat->callback((Fl_Callback*)evt_UpdateMulti, this);
  ui_rabbitDeath = new Fl_Spinner(20, 360, 170, 25, "Death multiplier:");
  ui_rabbitDeath->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_rabbitDeath->type(FL_FLOAT_INPUT);
  ui_rabbitDeath->range(0.0, 1.0);
  ui_rabbitDeath->step(0.05);
  ui_rabbitDeath->callback((Fl_Callback*)evt_UpdateMulti, this);

  ui_groupRabbitPrefs->end();

  /* Init fox prefs group */
  ui_groupFoxPrefs = new Fl_Group(210, 185, 190, 225, "Fox prefs");
  ui_groupFoxPrefs->box(FL_GTK_UP_FRAME);
  ui_groupFoxPrefs->align(Fl_Align(FL_ALIGN_TOP_LEFT));

  ui_foxGrow = new Fl_Spinner(220, 210, 170, 25, "Grow multiplier:");
  ui_foxGrow->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_foxGrow->type(FL_FLOAT_INPUT);
  ui_foxGrow->range(0.0, 100.0);
  ui_foxGrow->step(0.05);
  ui_foxGrow->callback((Fl_Callback*)evt_UpdateMulti, this);
  ui_foxMigration = new Fl_Spinner(220, 260, 170, 25, "Migration multiplier:");
  ui_foxMigration->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_foxMigration->type(FL_FLOAT_INPUT);
  ui_foxMigration->range(0.0, 100.0);
  ui_foxMigration->step(0.05);
  ui_foxMigration->callback((Fl_Callback*)evt_UpdateMulti, this);
  ui_foxEat = new Fl_Spinner(220, 310, 170, 25, "Eat multiplier:");
  ui_foxEat->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_foxEat->type(FL_FLOAT_INPUT);
  ui_foxEat->range(0.0, 100.0);
  ui_foxEat->step(0.05);
  ui_foxEat->callback((Fl_Callback*)evt_UpdateMulti, this);
  ui_foxDeath = new Fl_Spinner(220, 360, 170, 25, "Death multiplier:");
  ui_foxDeath->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_foxDeath->type(FL_FLOAT_INPUT);
  ui_foxDeath->range(0.0, 100.0);
  ui_foxDeath->step(0.05);
  ui_foxDeath->callback((Fl_Callback*)evt_UpdateMulti, this);

  ui_groupFoxPrefs->end();

  /* Init grass prefs group */
  ui_groupGrassPrefs = new Fl_Group(410, 290, 190, 120, "Grass prefs");
  ui_groupGrassPrefs->box(FL_GTK_UP_FRAME);
  ui_groupGrassPrefs->align(Fl_Align(FL_ALIGN_TOP_LEFT));

  ui_grassGrow = new Fl_Spinner(420, 315, 170, 25, "Grow multiplier:");
  ui_grassGrow->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_grassGrow->type(FL_FLOAT_INPUT);
  ui_grassGrow->range(0.0, 100.0);
  ui_grassGrow->step(0.05);
  ui_grassGrow->callback((Fl_Callback*)evt_UpdateMulti, this);
  ui_grassMigration =
      new Fl_Spinner(420, 365, 170, 25, "Migration multiplier:");
  ui_grassMigration->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  ui_grassMigration->type(FL_FLOAT_INPUT);
  ui_grassMigration->range(0.0, 100.0);
  ui_grassMigration->step(0.05);
  ui_grassMigration->callback((Fl_Callback*)evt_UpdateMulti, this);

  ui_groupGrassPrefs->end();

  /* Init outside UI elements  */
  buttonOpenView = new Fl_Button(420, 25, 180, 25, "Open simulation view");
  buttonOpenView->callback((Fl_Callback*)evt_OpenViewWindow, this);
  buttonPlay = new Fl_Button(420, 115, 85, 25, "Play");
  buttonPlay->callback((Fl_Callback*)evt_Run, this);
  buttonReset = new Fl_Button(515, 115, 85, 25, "Reset");
  buttonReset->callback((Fl_Callback*)evt_WorldReset, this);

#ifndef LESS_UI_MODE
  outCurrentTick = new Fl_Output(620, 140, 180, 25, "Current tick:");
  outCurrentTick->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  outTickRate = new Fl_Output(620, 90, 180, 25, "Real tick rate (per sec):");
  outTickRate->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  outStatus = new Fl_Output(620, 40, 180, 25, "Status:");
  outStatus->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  outStatus->value("standby");
  outStatus->textcolor(0xad6500ff);
#endif  // LESS_UI_MODE

  end();

  isViewOpened = false;
  isSimulationRun = false;
  world = new Simulator(ui_generalSizeX->value(), ui_generalSizeY->value());
  setDefault();
  reloadParams();
}

void UI::Console::evt_OpenViewWindow(Fl_Button* b, Console* v) {
  if (!v->isViewOpened) {
    v->windowView = new UI::View(v);
    v->windowView->show();
    v->isViewOpened = true;
    v->buttonOpenView->deactivate();
  }
}

void UI::Console::evt_SizesChanges(Fl_Spinner* s, Console* v) {
  v->worldReset();
}

void UI::Console::evt_WorldReset(Fl_Button* b, Console* v) { v->worldReset(); }

void UI::Console::evt_Run(Fl_Button* b, Console* v) {
#ifndef LESS_UI_MODE
  if (!v->isSimulationRun) {
    b->label("Pause");
    v->isSimulationRun = true;
    v->tick_tack = tclock::now();
    v->tacks = 0;
    Fl::add_timeout(1.0 / v->ui_generalTickRate->value(),
                    (Fl_Timeout_Handler)tickThread, v);
    v->outStatus->textcolor(0x1b9100ff);
    v->outStatus->value("simulation is running!");
#else
  Fl::add_timeout(0, (Fl_Timeout_Handler)tickThread, v);
#endif  // LESS_UI_MODE
  } else {
    b->label("Play");
    v->isSimulationRun = false;
#ifndef LESS_UI_MODE
    v->outStatus->textcolor(0xad6500ff);
    v->outStatus->value("standby");
#endif  // LESS_UI_MODE
  }
}

void UI::Console::evt_UpdateMulti(Fl_Spinner* s, Console* v) {
  v->reloadParams();
}

void UI::Console::evt_ResetToDefault(Fl_Button* b, Console* v) {
  v->setDefault();
  v->reloadParams();
}

void UI::Console::evt_CloseViewWindow(Console* v) {
  if (v->isViewOpened) {
    delete v->windowView;
    v->isViewOpened = false;
    v->buttonOpenView->activate();
  }
}

void UI::Console::worldReset() {
  evt_CloseViewWindow(this);
  delete world;
  world = new Simulator(ui_generalSizeX->value(), ui_generalSizeY->value());
  reloadParams();
}

void UI::Console::reloadParams() {
  world->grass_grow = ui_grassGrow->value();
  world->grass_expansion = ui_grassMigration->value();
  world->rabbit_grow = ui_rabbitGrow->value();
  world->rabbit_expansion = ui_rabbitMigration->value();
  world->rabbit_eat = ui_rabbitEat->value();
  world->rabbit_death = ui_rabbitDeath->value();
  world->fox_grow = ui_foxGrow->value();
  world->fox_expansion = ui_foxMigration->value();
  world->fox_eat = ui_foxEat->value();
  world->fox_death = ui_foxDeath->value();
}

void UI::Console::setDefault() {
  ui_grassGrow->value(0.1f);
  ui_grassMigration->value(0.1f);
  ui_rabbitGrow->value(0.1f);
  ui_rabbitMigration->value(0.08f);
  ui_rabbitEat->value(0.1f);
  ui_rabbitDeath->value(0.075f);
  ui_foxGrow->value(0.1f);
  ui_foxMigration->value(0.08f);
  ui_foxEat->value(0.1f);
  ui_foxDeath->value(0.08f);
}

void UI::Console::tickThread(Console* v) {
#ifndef LESS_UI_MODE
  v->world->tick();
  if (v->isViewOpened) v->windowView->worldUpdate();
  v->tacks++;
  std::chrono::duration<double> a =
      std::chrono::duration_cast<std::chrono::duration<double>>(tclock::now() -
                                                                v->tick_tack);
  if (a > std::chrono::seconds(1)) {
    v->outTickRate->value(std::to_string(v->tacks).c_str());
    v->tick_tack = tclock::now();
    v->tacks = 0;
  }
  v->outCurrentTick->value(std::to_string(v->world->currentTick()).c_str());
  if (v->isSimulationRun) {
    double delay_time = 1.0 / v->ui_generalTickRate->value();
    Fl::repeat_timeout(delay_time, (Fl_Timeout_Handler)tickThread, v);
  }
#else
  v->world->tick();
  if (v->isViewOpened) v->windowView->worldUpdate();
  if (v->isSimulationRun)
    Fl::repeat_timeout(0, (Fl_Timeout_Handler)tickThread, v);
#endif  // LESS_UI_MODE
}
