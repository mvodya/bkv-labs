// Описание работы с человеком
// by Mark Vodyanitskiy (@mvodya) 30.05.2019
#pragma once
#include <time.h>
#include "metaphone.h"
#include "utf8to1251.h"

struct PERSON {           // В кодировке CP1251
  char wfirst_name[128];  // Имя
  char mfirst_name[128];
  char wlast_name[128];  // Фамилия
  char mlast_name[128];
  char wmid_name[128];  // Отчество и т.п.
  char mmid_name[128];
  tm birthday;  // Дата рождения
  int age;      // Возраст

  // Декодирование строки из файла
  void parse(char* data);
  // Установка даты по трем числам
  void setDate(int day, int month, int year);
  // Генерация ФИО в формате метафора
  void genMetaphone();
  // Получение возраста
  int getAge();
  // Установка ФИО (с переводом в cp1251)
  void setFirstName(char* text) { utf8to1251(text, wfirst_name); }
  void setLastName(char* text) { utf8to1251(text, wlast_name); }
  void setMidName(char* text) { utf8to1251(text, wmid_name); }
  // Получение ФИО (с переводом в utf8)
  void getFirstName(char* out) { cp1251toUtf8(wfirst_name, out); }
  void getLastName(char* out) { cp1251toUtf8(wlast_name, out); }
  void getMidName(char* out) { cp1251toUtf8(wmid_name, out); }
  // Вывод основной инфы о пользователе
  void printInfo();
  // Кодирование строки для базы
  void encode(char* out);
};

void PERSON::parse(char* data) {
  int d, m, y;
  char secondbuf[513];
  // Читаем нужные данные из строки, пропуская некоторый мусор
  sscanf(data, "%256s %512[^0-9] %*d %*s %d %*d %*s %d.%d.%*d", wlast_name,
         secondbuf, &y, &d, &m);
  // Имя и отчество (два последющих слова) читаем вместе. Проблема в том, что
  // отчество может состоять из нескользих слов. %512[^0-9] прочитает все ровно
  // до первых цифр. На втором этапе отделим первое слово (имя) от отчества.
  // Т.к. отчетсва может не быть, заранее запишем в него нулевой символ
  // Имя и отчество по отдельности не читаются во избежании ситуации, когда
  // отчество отсутсвует.
  wmid_name[0] = '\0';
  // Отделяем имя от отчества
  sscanf(secondbuf, "%256s %[^0-9]", wfirst_name, wmid_name);
  // Устанавливаем дату рождения
  setDate(d, m, y);
  // Отдельный фикс с пробелами. В %512[^0-9] попадает пробел, который стоит
  // перед началом цифр.
  char* spacefix = wmid_name;
  while (*spacefix++ != '\0')
    ;
  // Помимо всего прочего нужно учесть ситуацию, когда отчества нету
  if (wmid_name[0] != '\0') spacefix[-2] = '\0';
  // Генерируем метафон
  genMetaphone();
}

void PERSON::setDate(int day, int month, int year) {
  // Получаем текущее время для локали (часового пояса)
  time_t rawtime;
  time(&rawtime);
  birthday = *localtime(&rawtime);
  // Записываем дату
  birthday.tm_mday = day;
  birthday.tm_mon = month - 1;
  birthday.tm_year = year - 1900;
  // Генерируем остальные части даты
  mktime(&birthday);
  age = getAge();
}

void PERSON::genMetaphone() {
  metaphone(wfirst_name, mfirst_name);
  metaphone(wlast_name, mlast_name);
  metaphone(wmid_name, mmid_name);
}

int PERSON::getAge() {
  // Получаем текущее время
  time_t rawtime;
  time(&rawtime);
  // Отнимаем из текущего времени дату рождения
  rawtime -= mktime(&birthday);
  // Записываем (формально получили возраст в секундах)
  return localtime(&rawtime)->tm_year - 70;
}

void PERSON::printInfo() {
  // Вывод информации о пользователе
  char date[16], last_name[257], first_name[257], mid_name[257];
  strftime(date, 10, "%d %b %y", &birthday);
  getLastName(last_name);
  getFirstName(first_name);
  getMidName(mid_name);
  printf("%s %s %s (%s) age: %d\n", last_name, first_name, mid_name, date, age);
}

// Сравнение пользователей по возрасту
int cmpPERSONage(PERSON** a, PERSON** b) { return (*a)->age - (*b)->age; }

// Сравнение пользователей по имени
int cmpPERSONabc(PERSON** a, PERSON** b) {
  char cmpBufA[768], cmpBufB[768];
  strcpy(cmpBufA, (*a)->wlast_name);
  strcat(cmpBufA, (*a)->wfirst_name);
  strcat(cmpBufA, (*a)->wmid_name);
  strcpy(cmpBufB, (*b)->wlast_name);
  strcat(cmpBufA, (*b)->wfirst_name);
  strcat(cmpBufA, (*b)->wmid_name);
  char *p1 = cmpBufA, *p2 = cmpBufB;
  int cmp = 0;
  // Перебираем слова по буквам
  while (*p1 != '\0' && *p2 != '\0') {
    // Сравниваем
    cmp = cmpCHARcyril(*p1, *p2);
    if (!cmp) {
      // Если буквы совпадают - продолжаем сравнение
      p1++;
      p2++;
      continue;
    }
    // Возвращаем ответ, если слова не равны
    return cmp;
  }
  if (!cmp) return cmpPERSONage(a, b);
  // Если одно из слов короче - наикратчайшее берем за большее
  return cmpCHARcyril(*p1, *p2);
}

void PERSON::encode(char* out) {
  char date[16], date2[16];
  strftime(date, 10, "%d %b", &birthday);
  strftime(date2, 10, "%d.%m.%y", &birthday);
  sprintf(out, "%s %s %s %s %d %d age %s", wlast_name, wfirst_name, wmid_name,
          date, (birthday.tm_year + 1900), age, date2);
}