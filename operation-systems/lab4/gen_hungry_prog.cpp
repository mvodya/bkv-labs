// Lab3 for operation system subject
// Modes for servicing short-term planning queues
// by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev

#include <iostream>
#include <queue>
#include <random>
#include <vector>

#define PROC_NUM 5000  // Число процессов

using namespace std;

struct proc {
  int start_time;  // Время старта процесса
  int end_time;    // Длительность процесса
  int page;  // Номер страницы, из с которой будет происходить тестовое чтение
             // (рандомное число)

  proc(int start_time) {
    this->start_time = start_time;
    this->end_time =
        rand() % 36 + 5;  // Продолжительность процесса от 5 до 40 случайно
    page = rand() % 10 + 1;  // Одна страница с номером от 1 до 10 случайно
  }

  // Конструктор с указанием длительности
  proc(int start_time, int end_time) {
    this->start_time = start_time;
    this->end_time = end_time;
    page = rand() % 10 + 1;
  }

  // Паттерн одного процесса для VisualOS
  friend ostream& operator<<(ostream& os, proc& p) {
    os << "proc {\n\tstart_time=" << p.start_time
       << "\n\tend_time=" << p.end_time
       << "\n\tIO{\n\t}\n\tMEM{\n\t\tpage=" << p.page << " write=0\n\t}\n}\n";
    return os;
  }
};

vector<proc*> process;  // Процессы
int timing = 0;  // Время работы исполняемого процесса
queue<int> timings;  // Очередь процессов (времени их работы)

int running_process = 0;
int tick = 1;

// Генератор мелких процессов
void generate_process() {
  proc* p = new proc(tick);
  process.push_back(p);
  timings.push(p->end_time);
  running_process++;
}

// Генератор длительных процессов
void generate_long_process(int time) {
  proc* p = new proc(tick, time);
  process.push_back(p);
  // timings.push(p->end_time);
  running_process++;
}

int main() {
  srand(0);

  // Генерация длительных процессов
  generate_long_process(600);
  generate_long_process(400);
  generate_long_process(500);

  while (process.size() < PROC_NUM) {
    // Генерация мелких процессов
    if (running_process < 15) generate_process();

    // Подсчет длительности работы процесса для защиты от переполнения
    if (timing <= 0) {
      timing = timings.front();
      timings.pop();
      running_process--;
    }
    timing--;
    tick++;
  }

  // Вывод процессов
  for (auto p : process) cout << *p;

  return 0;
}
