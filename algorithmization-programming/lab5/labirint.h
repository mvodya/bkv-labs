// Лабиринт
// by Mark Vodyanitskiy (@mvodya) 04.06.2019
#pragma once
#include <limits.h>
#include <stack>
#include "cell.h"

struct labirint {
  // n - столбцы
  // m - строки
  int n, m;
  // Ячейки лабиринта
  cell** cells;
  // Конструктор лабиринта
  labirint(int columns, int rows, char* data);
  // Проверка лабиринта на ошибки TODO
  // Распространение волны
  void waveExpansion();
  // Восстановление пути
  void backTrace();
  // Вывод кол-ва шагов для всех ячеек
  void printCellsWeights();
  // Вывод изображения лабиринта
  void printASCII();
  // Требуемое кол-во шагов для прохождения лабиринта
  int needSteps() { return cells[m - 1][n - 1].weight; }
};

labirint::labirint(int columns, int rows, char* data) {
  char* p = data;
  n = columns;
  m = rows;
  // Выделяем память под строки
  cells = new cell*[m];
  for (int i = 0; i < m; i++) {
    // Выделяем память под столбцы
    cells[i] = new cell[n];
    // Записываем границы в ячейки
    for (int j = 0; j < n; j++) {
      cells[i][j].setBounds(*p++);
      cells[i][j].weight = INT_MAX;
    }
  }
}

void labirint::waveExpansion() {
  int w = 0;
  bool end = false;
  // Стартовая ячейка
  cells[0][0].weight = 0;

  while (!end) {
    end = true;
    // Перебор всех ячеек
    for (int i = 0; i < m; i++)
      for (int j = 0; j < n; j++) {
        cell* p = &cells[i][j];
        // Поиск ячейки с числом w
        if (p->weight == w) {
          // Проверка границ
          if (!p->top() && cells[i - 1][j].weight > w)
            cells[i - 1][j].weight = w + 1;
          if (!p->right() && cells[i][j + 1].weight > w)
            cells[i][j + 1].weight = w + 1;
          if (!p->bottom() && cells[i + 1][j].weight > w)
            cells[i + 1][j].weight = w + 1;
          if (!p->left() && cells[i][j - 1].weight > w)
            cells[i][j - 1].weight = w + 1;
          end = false;
        }
      }
    w++;
  }
}

void labirint::printCellsWeights() {
  // Перебор всех ячеек
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      // Вывод ответа
      if (cells[i][j].weight != INT_MAX)
        printf("%-5d", cells[i][j].weight);
      else
        printf("%-5s", "X");
    }
    printf("\n");
  }
}

void labirint::printASCII() {
  int na = n * 2 + 1, ma = m * 2 + 1;
  // Выделяем память под строки
  char** buffer = new char*[ma];
  for (int i = 0; i < ma; i++) {
    // Выделяем память под столбцы
    buffer[i] = new char[na];
    // Заполняем буффер
    for (int j = 0; j < na; j++) buffer[i][j] = '#';
  }

  // Перебираем ячейки
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      cell* p = &cells[i][j];
      // Делаем саму ячейку пустой
      buffer[i * 2 + 1][j * 2 + 1] = ' ';
      // Проверка границ
      if (!p->top()) buffer[i * 2][j * 2 + 1] = ' ';
      if (!p->right()) buffer[i * 2 + 1][j * 2 + 2] = ' ';
      if (!p->bottom()) buffer[i * 2 + 2][j * 2 + 1] = ' ';
      if (!p->left()) buffer[i * 2 + 1][j * 2] = ' ';
    }
  }
  // Отрисовываем лабиринт
  for (int i = 0; i < ma; i++) {
    for (int j = 0; j < na; j++)
      // Двойной вывод символа
      printf("%c%c", buffer[i][j], buffer[i][j]);
    printf("\n");
  }
}

void labirint::backTrace() {
  // Берем конец лабиринта
  int x = n - 1, y = m - 1;
  std::stack<char> path;
  char side;

  int d = cells[y][x].weight;
  // Пока не вернулись на стартовую ячейку
  while (x || y) {
    d--;
    // Проверяем верх
    if (y != 0 && !cells[y][x].top() && cells[y - 1][x].weight == d) {
      side = 0;
      y--;
    }
    // Проверяем справа
    else if (x < n - 1 && !cells[y][x].right() && cells[y][x + 1].weight == d) {
      side = 1;
      x++;
    }
    // Проверяем снизу
    else if (y < m - 1 && !cells[y][x].bottom() && cells[y + 1][x].weight == d) {
      side = 2;
      y++;
    }
    // Проверяем слева
    else if (x != 0 && !cells[y][x].left() && cells[y][x - 1].weight == d) {
      side = 3;
      x--;
    }
    // Записываем выбранную сторону в стек
    path.push(side);
  }
  // Умышленно поменяны местами стороны, т.к. путь будет записан от старта к
  // фишишу, но расчитывается от финиша к старту
  printf("Path: ");
  while (!path.empty()) {
    // Получаем верхний элемент стека
    side = path.top();
    switch (side) {
      case 0:
        printf("down");
        break;
      case 1:
        printf("left");
        break;
      case 2:
        printf("up");
        break;
      case 3:
        printf("right");
        break;
      default:
        printf("err");
        break;
    }
    // Выкидываем верх стека
    path.pop();
    // Если стек еще не закончился, дорисовываем стрелочку в выводе
    if (!path.empty()) printf(" -> ");
  }
}