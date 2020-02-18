// Программа для поиска пути в лабиринте из точки начала (левый верхний угол) в
// точку финиша (правый нижний угол). Работает с стеночным лабиринтом, у
// которого стенки закодированы base 16.
// by Mark Vodyanitskiy (@mvodya) 04.06.2019

// Написано на C++ с использованием std::stack

#include <stdio.h>
#include <stdlib.h>
#include "labirint.h"

#define PRINT_LABIRINT
#define PRINT_WAVE

labirint* L;

// Читаем лабиринт
bool openLabirint(const char* filename) {
  int n = 0, m = 0;
  bool cloumnCounted = false;
  FILE* input = fopen(filename, "r");
  if (!input) return false;

  char *buffer = (char*)malloc(1000000 * sizeof(char)), *p = buffer;

  while (true) {
    // Читаем символ
    *p = fgetc(input);
    // Игнорируем новую строки и возврат каретки
    if (*p == '\r') continue;
    if (*p == '\n') {
      // Возвращаем на символ назад
      p--;
      // Подсчет кол-ва стобцов закончен
      if (!cloumnCounted) cloumnCounted = true;
      // Подсчитываем кол-во строк
      n++;
    }
    // Если конец чтения
    if (*p == EOF) {
      *p = '\0';
      break;
    }
    // Подсчитываем количество столбцов (только один раз)
    if (!cloumnCounted) m++;
    // Переходим на следующий символ в буффере
    p++;
  }
  L = new labirint(m, n, buffer);
  return true;
}

int main(int argc, char* argv[]) {
  // Требуем аргумент н авходе
  if (argc == 1) {
    printf("You must specify path of the labirint file to be processed.\n");
    return 1;
  }
  // Читаем лабиринт
  if (!openLabirint(argv[1])) {
    printf("Error while opening file %s\n", argv[1]);
    return 1;
  };
#ifdef PRINT_LABIRINT
  printf("Input labirint:\n");
  L->printASCII();
#endif  // PRINT_LABIRINT

  // Запускаем волну
  L->waveExpansion();
#ifdef PRINT_WAVE
  printf("\nWave expansion result:\n");
  L->printCellsWeights();
#endif  // PRINT_WAVE
  // Выводим нужное кол-во шагов
  if (L->needSteps() == INT_MAX) {
    printf("No way for this labirint\n");
    return 0;
  }
  printf("\nNeed steps: %d\n", L->needSteps());
  // Просчитываем обратный путь
  L->backTrace();
  printf("\n");
  return 0;
}