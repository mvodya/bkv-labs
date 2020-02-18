// Программа для частотного анализа текстовых файлов
// с поддержкой кодировки cp1251
// by Mark Vodyanitskiy (@mvodya) 20.05.2019
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Слово (cp1251)
typedef struct word {
  char* str;  // Массив букв
  int count;  // Количество, найденное в тексте
} WORD;

WORD* words[100000];  // Массив слов
int total;            // Всего слов в массиве

// Получает последовательно слово из файла
char* getWord(FILE* input);
// Читает и обрабатывает файл
void readFile(char* filename);

// Сравнение слов по частоте
int cmpWORDquant(WORD** w1, WORD** w2);
// Сравнение букв с учетом cp1251
int cmpCHARcyril(char a, char b);
// Сравнение слов по алфавиту
int cmpWORDabc(WORD** w1, WORD** w2);

int main(int argc, char* argv[]) {
  FILE *outABCD, *outFREQ;
  clock_t begin, end;
  double time_spent;

  // Проверяем наличие аргументов
  if (argc == 1) {
    printf("You must specify paths of the files to be processed.\n");
    return 1;
  }

  begin = clock();
  // Перебираем аргументы
  while (--argc) readFile(argv[argc]);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time spended: %lf sec\n\n", time_spent);

  // Сортировка по частоте
  begin = clock();
  printf("Sorting by alphabet...\n");
  qsort(words, total, sizeof(WORD*),
        (int (*)(const void*, const void*))cmpWORDabc);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time spended: %lf sec\n\n", time_spent);

  // Вывод ответа A
  outABCD = fopen("analysisA.txt", "w");
  fprintf(outABCD, "Total words: %d\n", total);
  for (int i = 0; i < total; i++)
    fprintf(outABCD, "%s - %d\n", words[i]->str, words[i]->count);

  // Сортировка по частоте
  begin = clock();
  printf("Sorting by counting...\n");
  qsort(words, total, sizeof(WORD*),
        (int (*)(const void*, const void*))cmpWORDquant);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time spended: %lf sec\n\n", time_spent);

  // Вывод ответа B
  outFREQ = fopen("analysisB.txt", "w");
  fprintf(outFREQ, "Total words: %d\n", total);
  for (int i = 0; i < total; i++)
    fprintf(outFREQ, "%s - %d\n", words[i]->str, words[i]->count);

  // Закрытие файлов
  fclose(outABCD);
  fclose(outFREQ);

  printf("Output: analysisA.txt and analysisB.txt\n");
  return 0;
}

// Является ли символ кириллической буквой?
inline char iscyrilic(int i) {
  // Учитывается буква Ё (аналог tolower - побитовое уножение на -17)
  if (i >= 192 || (i & (-17)) == 168) return 1;
  return 0;
}
// Перевод кириллической буквы в маленькую (с учетом Ё)
inline char tolowercyril(char i) { return (i == '\xa8') ? '\xb8' : (i | 32); }
// Проверка на специальные символы, принадлежащие словам
inline char isspecmid(char i) {
  return (i == '-') || (i == '`') || (i == '\'');
}

char* getWord(FILE* input) {
  // Выделяем память для ответа
  char *w = (char*)malloc(255 * sizeof(char)), *p = w;

  int c;
  char state = 0;
  char last;
  // Читаем слово
  while (1) {
    c = fgetc(input);
    if (c == EOF) {
      if (state == 0)
        // Если файл закончился, а новое слово не прочитано вернем NULL
        return NULL;
      else {
        // Если файл закончился, возвращаем слово
        *p++ = '\0';
        return w;
      }
    }
    if (!state && (isalpha(c) || iscyrilic(c))) {
      // Если встретилась буква
      state = 1;
    }
    if (state && (isalpha(c) || iscyrilic(c) || isspecmid(c) || isalnum(c))) {
      // Если встретилась буква или спец знак или цифры
      // Меняем регистр буквы на маленький
      if (isalpha(c))
        c = tolower(c);
      else if (iscyrilic(c))
        c = tolowercyril(c);
      // Записываем букву в массив
      *p++ = c;
    } else if (c = '\n' && last = '-')
      // Поддержка переносов
      p--;
    else if (state) {
      // Если попалась не буква, а слово читалось
      *p++ = '\0';
      return w;
    }
    last = c;
  }
  return NULL;
}

// Добавление слова в массив
void addWord(char* w) {
  // Проверяем, есть ли это слово в массиве?
  bool founded = false;
  for (int i = 0; i < total; i++) {
    char *c = words[i]->str, *p = c, *l = w;
    // Совпадает слово?
    while (*p != '\0' && *l != '\0') {
      if (*p != *l) break;
      p++;
      l++;
    }
    if (*p != *l) continue;
    founded = true;
    words[i]->count++;
    break;
  }
  if (founded) return;
  // Добавляем слово, если оно не найдено
  words[total] = (WORD*)malloc(sizeof(WORD));
  words[total]->str = w;
  words[total]->count = 1;
  total++;
}

void readFile(char* filename) {
  FILE* input;
  printf("Process file %s... ", filename);
  // Открываем файл для прочтения
  input = fopen(filename, "r");
  if (input == NULL) {
    // Если вдруг это не получилось
    printf("fail, file not found\n");
    return;
  }

  // Читаем файл и записываем слова
  while (1) {
    char* w = getWord(input);
    if (w == NULL) break;
    addWord(w);
  }
  // Закрываем файл
  fclose(input);

  printf("successfull\n");
}

int cmpWORDquant(WORD** w1, WORD** w2) {
  return ((*w1)->count - (*w2)->count) * -1;
}

inline int cmpCHARcyril(char a, char b) {
  // если a больше b - вернем положительное число
  // если a меньше b - вернем отрицательное число
  // если a равно b - вернем 0
  if (a == b) return 0;
  // отдельная работа с Ё
  if (a == '\xb8') return (unsigned char)b > 229 ? -1 : 1;
  if (b == '\xb8') return (unsigned char)a > 229 ? 1 : -1;
  // Для кириллических букв
  if (((unsigned char)a > 192 && b == '\0') ||
      ((unsigned char)b > 192 && a == '\0')) {
    return (a - b) * -1;
  }
  // В любой ситуации
  return (a - b);
}

int cmpWORDabc(WORD** w1, WORD** w2) {
  char *p1 = (*w1)->str, *p2 = (*w2)->str;
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
  // Если одно из слов короче - наикратчайшее берем за большее
  return cmpCHARcyril(*p1, *p2);
}