// Программа для частотного анализа текстовых файлов
// с поддержкой кодировки cp1251
// версия с бинарным деревом и поиском
// by Mark Vodyanitskiy (@mvodya) 30.05.2019
#include <ctype.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Слово (cp1251)
typedef struct word {
  char* str;  // Массив букв
  int count;  // Количество, найденное в тексте
} WORD;

int total;  // Всего слов в массиве

// Получает последовательно слово из файла
char* getWord(FILE* input);
// Читает и обрабатывает файл
void readFile(char* filename);

// Сравнение слов по частоте
int cmpWORDquant(WORD** w1, WORD** w2);
// Сравнение букв с учетом cp1251
int cmpCHARcyril(char a, char b);
// Сравнение слов по алфавиту
int cmpWORDabc(WORD* w1, WORD* w2);

struct NODE {
  WORD* w;
  NODE *left = NULL, *right = NULL;
};

NODE* tree = NULL;

FILE* outABCD;

// Перевод из UTF-8 в CP1251
void utf8to1251(char* in, char* out) {
  iconv_t i = iconv_open("CP1251", "UTF-8");
  size_t len = 256;
  iconv(i, &in, &len, &out, &len);
  iconv_close(i);
}

// Отсортированный вовод всех элементов дерева
void printTree(NODE* t) {
  if (t == NULL) return;
  printTree(t->left);
  fprintf(outABCD, "%s - %d\n", t->w->str, t->w->count);
  printTree(t->right);
}

// Поиск слова в дереве
WORD* treeFind(NODE* t, WORD* w) {
  if (t == NULL) return NULL;
  if (!cmpWORDabc(t->w, w)) return t->w;
  WORD* tmp;
  tmp = treeFind(t->left, w);
  if (tmp != NULL) return tmp;
  tmp = treeFind(t->right, w);
  if (tmp != NULL) return tmp;
}

int main(int argc, char* argv[]) {
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

  // Вывод ответа
  outABCD = fopen("analysisA.txt", "w");
  fprintf(outABCD, "Total words: %d\n", total);
  printTree(tree);
  printf("Output: analysisA.txt and analysisB.txt\n\n");

  // Закрытие файлов
  fclose(outABCD);

  WORD w;
  char input[256], encoded[256];
  while (true) {
    printf("Search: ");
    scanf("%256s", input);
    utf8to1251(input, encoded);
    w.str = encoded;
    WORD* ans = treeFind(tree, &w);
    if (ans == NULL) {
      printf("NOT FOUNDED\n");
      continue;
    }
    printf("%s - %d\n", input, ans->count);
  }

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

// Добавление слова в дерево
void addWord(char* w) {
  // Проверяем, есть ли это слово в дереве?
  bool founded = false;
  // Перебор дерева
  int lastCmp;
  NODE *n = tree, *last;
  WORD s;
  s.str = w;
  while (n != NULL) {
    int cmp = cmpWORDabc(&s, n->w) * -1;
    if (cmp > 0) {
      // Больше
      last = n;
      lastCmp = cmp;
      n = n->left;
    } else if (cmp < 0) {
      // Меньше
      last = n;
      lastCmp = cmp;
      n = n->right;
    } else {
      // Совпало
      founded = true;
      n->w->count++;
      break;
    }
  }
  if (founded) return;
  // Добавляем слово, если оно не найдено
  WORD* p = (WORD*)malloc(sizeof(WORD));
  p->str = w;
  p->count = 1;
  n = (NODE*)malloc(sizeof(NODE));
  n->w = p;
  n->left = n->right = NULL;
  if (lastCmp > 0)
    last->left = n;
  else if (lastCmp < 0)
    last->right = n;
  total++;
  if (tree == NULL) tree = n;
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

int cmpWORDabc(WORD* w1, WORD* w2) {
  char *p1 = w1->str, *p2 = w2->str;
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