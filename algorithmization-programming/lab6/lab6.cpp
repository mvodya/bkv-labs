// Программа для поиска оболочки из набора точек в 2D пространстве 
// by Mark Vodyanitskiy (@mvodya) 06.06.2019

#include <stdio.h>
#include <stdlib.h>
#include <stack>

struct point {
  float x, y;
  int number;
};
int count = 0;          // Количество точек
int memreserved = 100;  // На сколько точек выделено памяти
point* points;          // Точки

std::stack<point*> uphull;
std::stack<point*> downhull;

// Сравнение точек
int cmpPoint(const void* a, const void* b) {
  // Сравниваем по оси X, если совпадает, то сравниваем по оси Y
  return (*(point*)b).x - (*(point*)a).x;
}

// Векторное произведение AB * AC
bool vecX(point* a, point* b, point* c) {
  return a->x * (b->y - c->y) + b->x * (c->y - a->y) + c->x * (a->y - b->y) < 0;
}

// Алгоритм Грэхема
void grahamScan() {
  for (int i = 0; i < count; i++) {
    point *p1, *p2, *p3 = points + i;
    // Для верхней части
    while (uphull.size() >= 2) {
      // Вытаскиваем из стека последнюю точку
      p2 = uphull.top();
      uphull.pop();
      // И предпоследднюю
      p1 = uphull.top();
      if (vecX(p1, p2, p3)) {
        // Возвращаем точку назад, если с углами все в порядке
        uphull.push(p2);
        break;
      } else
        continue;
    }
    // Добавляем в стек
    uphull.push(p3);
    // Для нижней части
    while (downhull.size() >= 2) {
      // Вытаскиваем из стека последнюю точку
      p2 = downhull.top();
      downhull.pop();
      // И предпоследднюю
      p1 = downhull.top();
      if (!vecX(p1, p2, p3)) {
        // Возвращаем точку назад, если с углами все в порядке
        downhull.push(p2);
        break;
      } else
        continue;
    }
    // Добавляем в стек
    downhull.push(p3);
  }
}

// Чтение файла
bool readFile(const char* filename) {
  // Открытие файла
  FILE* file = fopen(filename, "r");
  if (!file) {
    printf("Error while opening file IN.txt\n");
    return false;
  }

  // Выделяем память на 100 точек
  points = (point*)malloc(sizeof(point) * memreserved);
  point* p = &points[0];
  // Читаем точки из файла
  while (fscanf(file, "%d %f %f", &p->number, &p->x, &p->y) > 0) {
    count++;
    // Веделяем память на +100 точек, как только ее начинает не хватать
    if (memreserved <= count) {
      memreserved += 100;
      points = (point*)realloc(points, sizeof(point) * memreserved);
    }
    // Смещаем указатель
    p = &points[count];
  }
  return true;
}

// Вывод ответа
void printAnswer(std::stack<point*> uphull, std::stack<point*> downhull) {
  printf("Hull points number: ");
  uphull.pop();
  while (!uphull.empty()) {
    printf("%d ", uphull.top()->number);
    uphull.pop();
  }
  std::stack<point*> rdownhull;
  while (downhull.size() > 0) {
    rdownhull.push(downhull.top());
    downhull.pop();
  }
  rdownhull.pop();
  while (!rdownhull.empty()) {
    printf("%d ", rdownhull.top()->number);
    rdownhull.pop();
  }
  printf("\n");
}

int main(int argc, char* argv[]) {
  // Требуем аргумент на входе
  if (argc == 1) {
    printf("You must specify path of file to be processed.\n");
    return 1;
  }
  // Читаем файл
  if (!readFile(argv[1])) return 1;

  // Сортировка точек по оси X
  qsort(points, count, sizeof(point), cmpPoint);
  // Запускаем построение оболочки
  grahamScan();

  // Выводим ответ
  printAnswer(uphull, downhull);
}