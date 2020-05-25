#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

enum { SPRINT = SIGALRM, SKILL = SIGUSR2 };

using namespace std;

pid_t don;           // Глава
pid_t pid[100];      // Дети
int pid_count;       // Количество детей
int generation = 0;  // Номер поколения ребенка

int tree_hight = 3;  // Высота дерева
int tree_children_count = 2;  // Количество детей при порождении дерева

void generate_child_tree(int hight) {
  // Не создаем дерево, если уже не нужно ._.
  if (hight == 1) {
    pid_count = 0;
    return;
  }

  for (int i = 0; i < tree_children_count; i++) {
    if ((pid[i] = fork()) == 0) {
      // Для ребенка
      generation++;
      generate_child_tree(hight - 1);
      // Цикл ребенка (вечный сон)
      while (1) sleep(1);
    }
  }
}

void print_tree(int s) {
  printf("pid: %d", getpid());
  if (getpid() != don) printf(" ppid: %d", getppid());
  printf(", gen: %d, children: ", generation);
  for (int i = 0; i < pid_count; i++) {
    printf("%d ", pid[i]);
  }
  if (pid_count == 0) printf("none");
  printf("\n");
  for (int i = 0; i < pid_count; i++) kill(pid[i], SPRINT);
}

void harakiri(pid_t p) {
  printf("%d", p);
  for (int i = 0; i < pid_count; i++) {
    kill(pid[i], SKILL);
  }
  for (int i = 0; i < pid_count; i++) {
    wait(NULL);
  }
  //sleep(1);
  exit(0);
  //raise(SIGTERM);
}

int main() {
  don = getpid();  // Записываем главу
  pid_count = tree_children_count;  // Ибо для всех при инициализации он один

  /* Обработчики сигналов */
  signal(SPRINT, print_tree);
  signal(SKILL, harakiri);

  // Делаем охлажденных детей
  generate_child_tree(tree_hight);

  int input;
  while (1) {
    switch (getchar()) {
      case 'p':  // Выводим всех детей
        raise(SPRINT);
        break;
      case 'k':  // Убиваем ребенка и его ребенка... и его ре...
        scanf("%d", &input);
        kill(input, SKILL);
        wait(NULL);
        break;
      case 'x':  // Самоликвидация мафии
        raise(SKILL);
        break;
    }
  }
}