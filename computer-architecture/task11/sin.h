#ifndef f_h
#define f_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
extern Fl_Input *coeff;
extern Fl_Input *coeff2;
#include <FL/Fl_Value_Input.H>
extern Fl_Value_Input *from;
extern Fl_Value_Input *to;
extern Fl_Value_Input *step;
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
extern Fl_Output *area;
extern Fl_Value_Input *delay;
extern Fl_Value_Input *n;
extern Fl_Button *plot;
#include <FL/Fl_Text_Display.H>
extern Fl_Text_Display *info;
#endif
