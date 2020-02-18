// Описание работы с ячейкой
// by Mark Vodyanitskiy (@mvodya) 04.06.2019
#pragma once
#include <stdlib.h>

struct cell {
  // Границы, записанные в 16-ом представлении
  //         1
  //      ███████
  //    8 ██   ██ 2
  //      ███████
  //         4
  char bounds;
  // Кол-во шагов, которое нужно сделать для достижения данной ячейки
  int weight;
  // Установка границ
  void setBounds(char b) {
    if (b >= '0' && b <= '9') bounds = b - '0';
    if (b >= 'A' && b <= 'F') bounds = b - 'A' + 11;
  }
  /* Проверка на наличие границ у клетки */
  inline bool top() { return bounds & 0b0001; };
  inline bool right() { return bounds & 0b00010; };
  inline bool bottom() { return bounds & 0b0100; };
  inline bool left() { return bounds & 0b1000; };
};
