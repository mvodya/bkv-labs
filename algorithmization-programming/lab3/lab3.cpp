// Программа база-данных пользователей с использованием
// линейного списка. Поиск поддерживает Metaphone.
// by Mark Vodyanitskiy (@mvodya) 30.05.2019

/*
 * ВНИМАНИЕ!
 * Код страшен, ужасен и опасен. Может вызвать эпилептические припадки (даже
 * если вы к такому не предрасположены). Вы были предупреждены, дальнейший
 * просмотр кода - на ваш страх и риск.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utf8to1251.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include "list.h"
#include "metaphone.h"
#include "person.h"

#define DATABASENAME "1_DATA.txt"

// Линейный список пользователей
list::LIST* L = NULL;
// Последний ID
int n = 0;

// Убираем из буффера \n
inline void clearbuf() {
  while (getchar() != '\n') continue;
}

// КОМАНДА: Сортировка по имени
void cmdSortAbc() { L = list::sortByName(L); }

// КОМАНДА: Сортировка по возрасту
void cmdSortAge() { L = list::sortByAge(L); }

// КОМАНДА: Поиск пользователя
void cmdSearch() {
  int mode = 0;
  printf(
      "(1) По ФИО\n(2) По дате рождения\n(3) По возрасту\nВыберите режим "
      "поиска: ");
  // Выбор режима поиска
  scanf("%d", &mode);
  clearbuf();
  // Устанавливаем указатель на начало списка для поиска
  list::LIST* p = L;
  switch (mode) {
    case 1:  // Поиск по ФИО
      char buffer[768], text[385];
      printf("ФИО: ");
      // Получаем ввод ФИО
      scanf("%768[^\n]", buffer);
      clearbuf();
      // Переводим в cp1251
      utf8to1251(buffer, text);
      // Переводим в формат метафона
      metaphone(text, buffer);

      char last_name[128], first_name[128], mid_name[128];
      last_name[0] = first_name[0] = mid_name[0] = '\0';
      sscanf(buffer, "%s %s %127[^\n]", last_name, first_name, mid_name);

      // Пробегаем по всему списку и ищем совпадения
      while (p != NULL) {
        // Поиск по фамилии
        if (strcmp(p->person->mlast_name, last_name)) {
          p = p->next;
          continue;
        }
        // Поиск по имени
        if (first_name[0] != '\0' &&
            strcmp(p->person->mfirst_name, first_name)) {
          p = p->next;
          continue;
        }
        // Поиск по отчеству
        if (mid_name[0] != '\0' && strcmp(p->person->mmid_name, mid_name)) {
          p = p->next;
          continue;
        }

        printf("id: %d | ", p->id);
        p->person->printInfo();
        p = p->next;
      }

      break;
    case 2:  // Поиск по дате рождения
    {
      // Ввод даты рождения
      int y = -1, m = -1, d = -1;
      printf("Дата рождения [dd mm yyyy]: ");
      char datebuf[256];
      scanf("%[^\n]", datebuf);
      sscanf(datebuf, "%d %d %d", &d, &m, &y);
      clearbuf();

      // Пробегаем по всему списку и ищем совпадения
      while (p != NULL) {
        if (d != -1 && p->person->birthday.tm_mday != d) {
          p = p->next;
          continue;
        }
        if (m != -1 && p->person->birthday.tm_mon != (m - 1)) {
          p = p->next;
          continue;
        }
        if (y != -1 && p->person->birthday.tm_year != (y - 1900)) {
          p = p->next;
          continue;
        }

        printf("id: %d | ", p->id);
        p->person->printInfo();
        p = p->next;
      }
    } break;
    case 3:  // Поиск по возрасту
    {
      // Ввод возраста
      int age;
      printf("Возраст: ");
      scanf("%d", &age);
      clearbuf();

      // Пробегаем по всему списку и ищем совпадения
      while (p != NULL) {
        if (p->person->age != age) {
          p = p->next;
          continue;
        }

        printf("id: %d | ", p->id);
        p->person->printInfo();
        p = p->next;
      }
      break;
    }
    default:
      break;
  }
}

// КОМАНДА : Добавление пользователя
void cmdAdd() {
  // Выделяем память под пользователя
  PERSON* p = (PERSON*)malloc(sizeof(PERSON));
  char in[257];
  // Ввод данных ФИО
  printf("Фамилия: ");
  scanf("%256s", in);
  p->setLastName(in);
  printf("Имя: ");
  scanf("%256s", in);
  p->setFirstName(in);
  printf("Отчество: ");
  clearbuf();
  scanf("%256[^\n]", in);
  p->setMidName(in);
  // Ввод даты рождения
  int y, m, d;
  printf("Дата рождения [dd mm yyyy]: ");
  scanf("%d %d %d", &d, &m, &y);
  p->setDate(d, m, y);
  clearbuf();
  // Генерируем метафон
  p->genMetaphone();
  // Добавляем пользователя в список
  L = list::add(L, p, n++);

  // Вывод информации о пользователе, которого добавили
  char date[16], last_name[257], first_name[257], mid_name[257];
  strftime(date, 10, "%d %b %y", &p->birthday);
  p->getLastName(last_name);
  p->getFirstName(first_name);
  p->getMidName(mid_name);
  printf("\nДОБАВЛЕН:\n  ФИО: %s %s %s\n  ДАТА РОЖДЕНИЯ: %s\n  ВОЗРАСТ: %d\n\n",
         last_name, first_name, mid_name, date, p->age);
}

// КОМАНДА : Удаление пользователя
void cmdRm() {
  // Ввод ID
  int id;
  printf("id: ");
  scanf("%d", &id);
  clearbuf();
  // Проверка ID
  if (id > n) {
    printf("ID слишком большой, такого пользователя в базе нет\n");
    return;
  }

  // Удаляем пользователя из списка
  L = list::removeID(L, id);
  printf("Пользователь удален\n");
}

// КОМАНДА : Удаление всей базы пользователей
void cmdRmAll() {
  char i;
  printf("Вы уверены? (y/N): ");
  scanf("%c", &i);
  clearbuf();
  if (i != 'y') return;
  printf("Удаление базы... ");
  fflush(stdout);

  // Удаляем головной элемент из списка до тех пор, пока список не станет пустым
  do
    L = list::rmhead(L);
  while (!isEmpty(L));
  // Обнуляем последний ID
  n = 0;

  printf("УДАЛЕНА\n");
}

// КОМАНДА : Список всех пользователей
void cmdList() {
  // Проверяем, пустой ли список?
  if (list::isEmpty(L)) {
    printf("База пользователей пустая\n");
    return;
  }
  printf("Весь список пользователей в базе:\n");
  // Выводим всех пользователей базы
  list::LIST* p = L;
  while (!list::isEmpty(p)) {
    char date[16];
    strftime(date, 10, "%d %b %y", &p->person->birthday);
    char last_name[257], first_name[257], mid_name[257];
    p->person->getLastName(last_name);
    p->person->getFirstName(first_name);
    p->person->getMidName(mid_name);
    printf("id:%d %s %s %s (%s) age: %d\n", p->id, last_name, first_name,
           mid_name, date, p->person->age);
    p = p->next;
  }
}

// Загрузка базы из файла
bool loadDatabase(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    printf("ОШИБКА\nПроверьте доступность файла базы %s\n", filename);
    return false;
  }

  char line[500];
  while (fgets(line, 500, file) != NULL) {
    PERSON* p = (PERSON*)malloc(sizeof(PERSON));
    p->parse(line);
    L = list::add(L, p, n++);
  }

  fclose(file);
  printf("ОК (%d записей)\n", n);
  return true;
}

// Сохранение базы в файл
bool saveDatabase(const char* filename) {
  FILE* file = fopen(filename, "w");
  if (!file) {
    printf("ОШИБКА\nПроверьте доступность файла базы %s\n", filename);
    return false;
  }

  list::LIST* p = L;
  char buf[800];
  while (!list::isEmpty(p)) {
    p->person->encode(buf);
    fprintf(file, "%s\n", buf);
    p = p->next;
  }
  fclose(file);
  printf("ОК (%d записей)\n", n);
  return true;
}

// КОМАНДА: Сортировка по имени
void cmdSave() {
  printf("Сохранение списка пользователей... ");
  fflush(stdout);
  saveDatabase(DATABASENAME);
}

// Цикл работы с консолью
void console() {
  char input[256];
  while (1) {
    printf("~ ");
    fgets(input, 256, stdin);
    input[0] = tolower(input[0]);
    // help add rm rmall search sortabc sortdate list save quit kill

    if (!strcmp(input, "help\n"))
      printf(
          "Команды:\n  help - показать этот текст\n  add - добавить запись\n  "
          "rm - удалить запись  \n  rmall - удалить все записи  \n  search - "
          "поиск по имени, фамилии, дате рождения или возрасте\n  sortabc - "
          "сортировка по имени абонента\n  sortage - сортировка по возрасту\n"
          "  list - вывести все записи\n  save - сохранить "
          "базу в файл\n  quit - завершить работу с базой и сохранить ее в "
          "файл\n  kill - завершить работу не сохраняя базу\n");
    else if (!strcmp(input, "add\n"))
      cmdAdd();
    else if (!strcmp(input, "list\n") || !strcmp(input, "ls\n"))
      cmdList();
    else if (!strcmp(input, "rm\n"))
      cmdRm();
    else if (!strcmp(input, "rmall\n"))
      cmdRmAll();
    else if (!strcmp(input, "search\n") || !strcmp(input, "s\n"))
      cmdSearch();
    else if (!strcmp(input, "sortabc\n"))
      cmdSortAbc();
    else if (!strcmp(input, "sortage\n"))
      cmdSortAge();
    else if (!strcmp(input, "save\n"))
      cmdSave();
    else if (!strcmp(input, "quit\n"))
      return;
    else if (!strcmp(input, "kill\n"))
      exit(0);
    else
      printf("Команда не опознана, попробуйте ~ help\n");
  }
}

int main() {
#ifdef _WIN32
  system("chcp 65001 && cls");
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
#endif  // _WIN32

  printf(
      "Предупреждение: файл базы данных должен быть в кодировке "
      "cp1251.\n\nЗагрузка списка пользователей... ");
  fflush(stdout);
  if (!loadDatabase(DATABASENAME)) return 1;

  // Открытие цикла работы
  console();

  // Сохранение данных в файл
  printf("Сохранение списка пользователей... ");
  fflush(stdout);
  saveDatabase(DATABASENAME);

  return 0;
}