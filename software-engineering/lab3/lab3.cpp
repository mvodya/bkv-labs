// Software engineering
// Lab 3
// @mvodya 07/10/19

// REQUIRE GUI LIBRARY: FLTK 1.3

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif  // _WIN32

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Light_Button.H>
#include <iostream>
#include <sstream>
#include <string>
#include <climits>

// Главное окно
Fl_Double_Window* mainWindow;

// Кнопки окна
Fl_Button* buttonOperation;
Fl_Button* labelA;
Fl_Button* labelB;
Fl_Button* labelC;
Fl_Button* labelCF;
Fl_Button* subLabelCF;
Fl_Input* labelAconv;
Fl_Input* labelBconv;
Fl_Input* labelCconv;
Fl_Button* buttonFormat;
Fl_Button* buttonCalculate;

// Группы кнопок битов
Fl_Light_Button* buttonA_number[16];
Fl_Button* buttonA_number_labels[16];
Fl_Light_Button* buttonB_number[16];
Fl_Button* buttonB_number_labels[16];
Fl_Light_Button* buttonC_number[16];
Fl_Button* buttonC_number_labels[16];

// Числа A B C
short int a = 0, b = 0, c = 0;
bool carrige_flag = 0;

// Текущая операция
static enum OPERATION { PLUS, MINUS, MUL, DIV, LSHIFT, RSHIFT, AND, OR, NOT } operation;
static const std::string operations[] = {"+", "-", "*", "/", "<<", ">>", "&", "|", "~"};
// Формат конвертированного вывода
static enum FORMAT { DEC, OCT, HEX } format;
static const std::string formats[] = {"DEC", "OCT", "HEX"};

// Выполнение выбранной операции
void calculate() {
  std::stringstream stream;
  carrige_flag = 0;
  int i = 16;
  short int tmp = a;
  switch (operation) {
    case PLUS:
      c = a + b;
      if (a > SHRT_MAX - b) carrige_flag = 1;
      break;
    case MINUS:
      c = a - b;
      if (a < SHRT_MIN + b) carrige_flag = 1;
      break;
    case MUL:
      c = a * b;
      if (a > SHRT_MAX / b) carrige_flag = 1;
      if (a < SHRT_MIN / b) carrige_flag = 1;
      break;
    case DIV:
      // zero protect
      if (b) c = a / b;
      break;
    case LSHIFT:
      c = a << b;
      // ищем позицию поледнего бита
      while (--i && !(tmp & SHRT_MIN)) tmp <<= 1;
      if (i + b >= 16)
        carrige_flag = 1;
      break;
    case RSHIFT:
      c = a >> b;
      break;
    case AND:
      c = a & b;
      break;
    case OR:
      c = a | b;
      break;
    case NOT:
      c = ~a;
      break;
    default:
      break;
  }
}

// Перевод числа под текущий формат
std::string formatter(short int value) {
  std::stringstream stream;
  switch (format) {
    case DEC:
      stream << std::dec << value;
      break;
    case OCT:
      stream << std::oct << value;
      break;
    case HEX:
      stream << std::hex << value;
      break;
    default:
      break;
  }
  return stream.str();
}

// Перевод из текущего формата
short int deformatter(std::string value) {
  std::stringstream stream;
  stream << value;
  short int x = 0;
  switch (format) {
    case DEC:
      stream >> std::dec >> x;
      break;
    case OCT:
      stream >> std::oct >> x;
      break;
    case HEX:
      stream >> std::hex >> x;
      break;
    default:
      break;
  }
  return x;
}

// Вызовы
namespace callback {
// Смена операции
static void buttonOperationClick(Fl_Button*, void*) {
  operation = OPERATION((operation + 1) % (NOT + 1));
  buttonOperation->label(operations[(int)operation].c_str());
}

// Выполнение операции
static void buttonCalculateClick() {
  calculate();
  // Вывод кнопок C
  for (int i = 0; i < 16; i++) buttonC_number[i]->value(c & (1 << (15 - i)));

  labelCconv->value(formatter(c).c_str());
  if (carrige_flag) labelCF->label("1");
  else labelCF->label("0");
}

// Ручной ввод значения
static void labelConvUpdate(Fl_Button*, void*) {
  a = deformatter(labelAconv->value());
  b = deformatter(labelBconv->value());

  // Вывод кнопок A B C
  for (int i = 0; i < 16; i++) {
    buttonA_number[i]->value(a & (1 << (15 - i)));
    buttonB_number[i]->value(b & (1 << (15 - i)));
  }
}

// Смена формата вывода
static void buttonFormatClick(Fl_Button*, void*) {
  format = FORMAT((format + 1) % (HEX + 1));
  buttonFormat->label(formats[(int)format].c_str());

  // Вывод кнопок A B C
  for (int i = 0; i < 16; i++) {
    buttonA_number[i]->value(a & (1 << (15 - i)));
    buttonB_number[i]->value(b & (1 << (15 - i)));
    buttonC_number[i]->value(c & (1 << (15 - i)));
  }

  labelAconv->value(formatter(a).c_str());
  labelBconv->value(formatter(b).c_str());
  labelCconv->value(formatter(c).c_str());
}

// Клик по кнопке бита
static void buttonNumberClick() {
  a = 0;
  b = 0;
  // Чтение кнопок A и B
  for (int i = 0; i < 16; i++) {
    a += buttonA_number[i]->value() * (1 << (15 - i));
    b += buttonB_number[i]->value() * (1 << (15 - i));
  }
  // Вывод кнопок C
  for (int i = 0; i < 16; i++) buttonC_number[i]->value(c & (1 << (15 - i)));

  labelAconv->value(formatter(a).c_str());
  labelBconv->value(formatter(b).c_str());
  labelCconv->value(formatter(c).c_str());
}
}  // namespace callback

// Создаем группу кнопок для вывода 8бит числа
void init8bitGroup(Fl_Light_Button* button[], int height) {
  for (int i = 0; i < 16; i++) {
    button[i] = new Fl_Light_Button(80 + (18 * (i + 1) + (18 * (i / 8))),
                                    height, 16, 35);
    button[i]->box(FL_FLAT_BOX);
    button[i]->labelsize(25);
    button[i]->callback((Fl_Callback*)callback::buttonNumberClick);

    // Создаем подпись для кнопки
    std::string* label = new auto(std::to_string(15 - i));
    // Указатель label не удаляется и теряется в недрах памяти... (так н-нада)
    // Почему? Потому что FL_Button не копирует себе значение а юзает ссылку.
    buttonA_number_labels[i] =
        new Fl_Button(80 + (18 * (i + 1) + (18 * (i / 8))), height + 35, 16, 10,
                      label->c_str());
    buttonA_number_labels[i]->box(FL_FLAT_BOX);
    buttonA_number_labels[i]->labelsize(10);
    buttonA_number_labels[i]->set_output();
  }
}

// Создаем все элементы интерфейса
void initUI() {
  buttonOperation = new Fl_Button(25, 55, 35, 35, "+");
  buttonOperation->box(FL_FLAT_BOX);
  buttonOperation->labelsize(25);
  buttonOperation->callback((Fl_Callback*)callback::buttonOperationClick);

  labelA = new Fl_Button(60, 20, 35, 35, "a");
  labelA->box(FL_FLAT_BOX);
  labelA->labelsize(25);
  labelA->set_output();
  init8bitGroup(buttonA_number, 20);

  labelB = new Fl_Button(60, 90, 35, 35, "b");
  labelB->box(FL_FLAT_BOX);
  labelB->labelsize(25);
  labelB->set_output();
  init8bitGroup(buttonB_number, 90);

  labelC = new Fl_Button(60, 160, 35, 35, "c");
  labelC->box(FL_FLAT_BOX);
  labelC->labelsize(25);
  labelC->set_output();
  init8bitGroup(buttonC_number, 160);
  for (int i = 0; i < 16; i++) buttonC_number[i]->set_output();

  labelCF = new Fl_Button(25, 160, 35, 35, "0");
  labelCF->box(FL_FLAT_BOX);
  labelCF->labelsize(25);

  labelAconv = new Fl_Input(425, 20, 65, 35);
  labelAconv->callback((Fl_Callback*)callback::labelConvUpdate);
  labelAconv->when(FL_WHEN_CHANGED);
  labelAconv->value("0");

  labelBconv = new Fl_Input(425, 90, 65, 35);
  labelBconv->callback((Fl_Callback*)callback::labelConvUpdate);
  labelBconv->when(FL_WHEN_CHANGED);
  labelBconv->value("0");

  labelCconv = new Fl_Input(425, 160, 65, 35);
  labelCconv->callback((Fl_Callback*)callback::labelConvUpdate);
  labelCconv->when(FL_WHEN_CHANGED);
  labelCconv->value("0");
  labelCconv->set_output();

  subLabelCF = new Fl_Button(5, 165, 30, 20, "CF:");
  subLabelCF->box(FL_FLAT_BOX);
  subLabelCF->labelsize(10);

  buttonFormat = new Fl_Button(425, 205, 65, 35, "DEC");
  buttonFormat->box(FL_FLAT_BOX);
  buttonFormat->labelsize(25);
  buttonFormat->callback((Fl_Callback*)callback::buttonFormatClick);

  buttonCalculate = new Fl_Button(25, 125, 35, 35, "=");
  buttonCalculate->box(FL_FLAT_BOX);
  buttonCalculate->labelsize(25);
  buttonCalculate->shortcut(0x3d);
  buttonCalculate->callback((Fl_Callback*)callback::buttonCalculateClick);
}

int main(int argc, char** argv) {
  // Создаем главное окно
  mainWindow = new Fl_Double_Window(519, 256, "Binary calculator");
  if (!mainWindow) {
    // Если вдруг не удалось создать окно
    std::cout << "Fail! Can't init GUI window.";
    return -1;
  }
  initUI();
  mainWindow->end();
  mainWindow->show(argc, argv);
  return Fl::run();
}