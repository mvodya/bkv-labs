#include <stdio.h>
#include <math.h>
#include <FL/Fl.H>
#include <FL/Fl_Overlay_Window.H>
#include <FL/Fl_Light_Button.H>
#include "Cartesian.H"
#include <FL/fl_draw.H>
#include "sin.h"
#define SIZE 3600
//*********************  УПРАВЛЕНИЕ РАЗМЕРОМ МАССИВА - ***********************
#undef SIZE
extern "C" void calcul(void);

const double MIN_X=0;
const double MAX_X=10;
Ca_X_Axis*  x;
Ca_Y_Axis*  y;
  int TextNo=0;
double DELAY=0.001, FROM=0, TO=2*M_PI, STEP=0.01;
 int N;
extern short int HARM;
#ifdef SIZE 
extern double SINE[];
#else
extern float SINE[];
#define SIZE 360
#endif
Ca_Canvas *canvas;
double X=MIN_X,Y=0;
    static Ca_LinePoint *P_I=0, *PP_I;
void  next_point(void *){
    static int ii=0;
    static double X_STEP=1;
    Y = SINE[ii++];
    y->rescale(CA_WHEN_MAX,Y);    //different rescalling for max and min  
    y->rescale(CA_WHEN_MIN,Y);    //above maximum
    x->rescale(CA_WHEN_MAX,X);    //different rescalling for max and min  
    x->rescale(CA_WHEN_MIN,X);    //above maximum
    P_I=new Ca_LinePoint(P_I,X,Y,0,FL_BLUE,CA_NO_POINT);
    X += X_STEP;
        if (ii==(SIZE+1))
        {
          ii=0; X=MIN_X; 
          return;
        }
        Fl::add_timeout(DELAY,next_point);
};


Fl_Input *coeff=(Fl_Input *)0;
Fl_Input *coeff2=(Fl_Input *)0;

static void cb_coeff(Fl_Input*, void*) {
  ;
}

Fl_Value_Input *from=(Fl_Value_Input *)0;

static void cb_from(Fl_Value_Input*, void*) {
  FROM=from->value();
}

Fl_Value_Input *to=(Fl_Value_Input *)0;

static void cb_to(Fl_Value_Input*, void*) {
  TO=to->value();
}

Fl_Value_Input *step=(Fl_Value_Input *)0;

static void cb_step(Fl_Value_Input*, void*) {
  STEP=step->value();
}

Fl_Output *area=(Fl_Output *)0;

Fl_Value_Input *delay=(Fl_Value_Input *)0;

static void cb_delay(Fl_Value_Input*, void*) {
  DELAY=delay->value();
  }

Fl_Value_Input *n=(Fl_Value_Input *)0;

Fl_Button *plot=(Fl_Button *)0;

static void cb_plot(Fl_Button*, void*) {
    HARM=n->value();
    while ((PP_I=P_I,P_I=P_I->previous)!=NULL)delete(PP_I) ;
     delete(PP_I);
    N=361; calcul()  ;
    Fl::add_timeout(0,next_point);
}

static void cb_n(Fl_Value_Input*nn, void*vv) {
 HARM =  n->value();
  //  cb_plot((Fl_Button *)nn,vv);
//    N=361; calcul()  ;
}

Fl_Text_Display *info=(Fl_Text_Display *)0;

static void cb_info(Fl_Text_Display*, void*) {
  ;
}

int main(int argc, char **argv) {
  Fl_Double_Window* w;
  { Fl_Double_Window* w = new Fl_Double_Window(810, 435, "Вычисление ряда Фурье");
   
    w->labelsize(24);
    w->labelcolor((Fl_Color)1);
    { coeff = new Fl_Input(30, 20, 215, 40, "coefficient (k) при cos(kx)");
      coeff->tooltip("Формула для вычисления коэффициентов");
      coeff->callback((Fl_Callback*)cb_coeff);
      coeff->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Input* coeff
    { from = new Fl_Value_Input(30, 140, 65, 25, "from");
     from->tooltip("Пределы по Х - начало");
      from->maximum(6.29);
      from->step(0.1);
      from->callback((Fl_Callback*)cb_from);
      from->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Value_Input* from
    { to = new Fl_Value_Input(95, 140, 65, 25, "to");
      to->tooltip("Пределы по Х - конец");
      to->maximum(6.29);
      to->step(0.1);
      to->value(6.28);
      to->callback((Fl_Callback*)cb_to);
      to->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Value_Input* to
    { step = new Fl_Value_Input(160, 140, 65, 25, "step");
      step->tooltip("Изменение по Х - шаг");
      step->minimum(0.01);
      step->maximum(0.2);
      step->step(0.01);
      step->value(0.1);
      step->callback((Fl_Callback*)cb_step);
      step->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Value_Input* step
    { delay = new Fl_Value_Input(30, 180, 65, 25, "Delay");
      delay->tooltip("Задержка при построении графика");
      delay->minimum(0.001);
      delay->step(0.001);
      delay->value(0.001);
      delay->callback((Fl_Callback*)cb_delay);
      delay->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Value_Input* delay
    { n = new Fl_Value_Input( 115, 180, 65, 25, "N");
      n->tooltip("Количество гармоник");
      n->minimum(1);
      n->maximum(1000);
      n->step(1);
      n->value(7);
      n->callback((Fl_Callback*)cb_n);
      n->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Value_Input* n
    { plot = new Fl_Button(30, 210, 215, 50, "PLOT");
      plot->tooltip("Нажать для построения графика");
      plot->labelsize(42);
      plot->callback((Fl_Callback*)cb_plot);
    } // Fl_Button* plot
    {
       info = new Fl_Text_Display(30, 275, 215, 155);
      info->callback((Fl_Callback*)cb_info);
      Fl_Text_Buffer *tbuff = new Fl_Text_Buffer();        // text buffer
   info->buffer(tbuff);
   tbuff->text("Ряд Фурье\nдля пилы\n"
               "sin(x) - 1/2 * sin(2*x)\n      + 1/3 *sin(3*x)\n      - 1/4 * sin(4*x)+...\n");
      
    } // Fl_Text_Display* info

    { coeff2 = new Fl_Input(30, 80, 215, 40, "coefficient (k) при cos(kx)");
      coeff2->tooltip("Формула для вычисления коэффициентов");
      coeff2->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Input* o
    Fl_Group *c =new Fl_Group(250, 0, 545, 430 );
    c->box(FL_DOWN_BOX);
    c->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);

    canvas = new Ca_Canvas(270, 10, 540, 400, "");
    canvas->box(FL_DOWN_BOX);
    canvas->color(7);
    canvas->align(FL_ALIGN_TOP);
    Fl_Group::current()->resizable(canvas);
    canvas->border(15);

    x = new Ca_X_Axis(270, 400, 560, 20, "");
    x->align(FL_ALIGN_BOTTOM);
    x->minimum(MIN_X);
    x->maximum(MAX_X);
    x->label_format("%g");
    x->minor_grid_color(fl_gray_ramp(20));
    x->major_grid_color(fl_gray_ramp(15));
    x->label_grid_color(fl_gray_ramp(10));
    x->major_step(10);
        x->axis_color(FL_BLACK);
        x->axis_align(CA_BOTTOM|CA_LINE);

    y = new Ca_Y_Axis(250, 10, 20, 400, "");
    y->align(FL_ALIGN_TOP_RIGHT);
    y->minimum(-1); y->maximum(1);
    y->major_step(1);
    Fl_Group::current()->resizable(c);
    w->resizable(w);
    w->end();
    w->show();
    HARM=n->value();
    if ( SIZE == 360 )
      N=360;
    else N=3600;
     calcul();
    from->hide(); to->hide(); step->hide(); 
    coeff->hide();
    coeff2->hide();
    Fl::add_timeout(0,next_point);
        Fl::run();
        return 0;
   }
}
