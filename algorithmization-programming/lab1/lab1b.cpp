// Программа для перевода транслита в русский текст по ISO 9-95
// by Mark Vodyanitskiy (@mvodya) 15.05.2019
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define L2 '\x12'  // двухбуквенная последовательность
#define L3 '\x13'  // трехбуквенная последовательность
#define LATIN '\x14'  // режим латиницы
#define CYRIL '\x15'  // режим кириллицы

// Таблица перевода с латиницы на кириллицу
// cp-1251
const char fromtrl[] = {
    '\xe0',  // a - 97
    '\xe1',  // b - 98
    '\xf6',  // c - 99
    '\xe4',  // d - 100
    '\xe5',  // e - 101
    '\xf4',  // f - 102
    '\xe3',  // g - 103
    '\xf5',  // h - 104
    '\xe8',  // i - 105
    '\xe9',  // j - 106
    '\xea',  // k - 107
    '\xeb',  // l - 108
    '\xec',  // m - 109
    '\xed',  // n - 110
    '\xee',  // o - 111
    '\xef',  // p - 112
    '?',     // q - 113
    '\xf0',  // r - 114
    '\xf1',  // s - 115
    '\xf2',  // t - 116
    '\xf3',  // u - 117
    '\xe2',  // v - 118
    '?',     // w - 119
    '\xf5',  // x - 120
    '\xfb',  // y - 121
    '\xe7',  // z - 122
};

// Перевод файла из транслита
void cyrilic(char* filename);
// Перевод последовательности на кириллицу
char decode(int* i, char state, int last);

int main(int argc, char** argv) {
  // Проверяем наличие аргументов
  if (argc == 1) {
    printf("You must specify paths of the files to be processed.\n");
    return 1;
  }

  // Перебираем аргументы
  while (--argc) cyrilic(argv[argc]);

  return 0;
}

// Является ли символ кириллической буквой?
inline char iscyrilic(int i) {
  // Учитывается буква Ё (аналог tolower - побитовое уножение на -17)
  if (i >= 192 || (i & (-17)) == 168) return 1;
  return 0;
}
// Является ли символ большой кириллической буквой?
inline char isupcyril(int i) {
  if (i < 224 && i != 184) return 1;
  return 0;
}
// Перевод кириллической буквы в большую (с учерем Ё)
inline char toupcyril(char i) { return (i == '\xb8') ? '\xa8' : (i & 0xdf); }
// Смещение кода латинского символа для таблицы totrl
inline int convto(int i) { return tolower(i) - (char)97; }

void cyrilic(char* filename) {
  FILE *input, *output;
  printf("Process file %s... ", filename);
  // Открываем файл для прочтения
  input = fopen(filename, "r");
  if (input == NULL) {
    // Если вдруг это не получилось
    printf("fail, file not found\n");
    return;
  }
  // Создаем имя выходного файла
  char outfilename[255];
  strcpy(outfilename, filename);
  strcat(outfilename, ".trl.txt");
  // Открываем файл для записи
  output = fopen(outfilename, "w");
  if (output == NULL) {
    // Если вдруг не получилось
    printf("fail, can't open %s for out\n", outfilename);
    return;
  }
  int a[4], last = 0;
  char buf, mode = CYRIL;
  // Читаем посимвольно файл input до EOF
  while ((a[0] = fgetc(input)) != EOF) {
    if (mode == LATIN && a[0] != CYRIL) {
      // Если режим латиницы, сразу выводим символ
      fputc(a[0], output);
      continue;
    }
    // Проверяем наличие флагов L2, L3, LATIN и CYRIL
    switch (a[0]) {
      case L2:
        // Обработка двойной последовательности
        a[1] = fgetc(input);
        a[2] = fgetc(input);
        fputc(decode(a, L2, last), output);
        last = a[1];
        break;
      case L3:
        // Обработка тройной последовательности
        a[1] = fgetc(input);
        a[2] = fgetc(input);
        a[3] = fgetc(input);
        fputc(decode(a, L3, last), output);
        last = a[1];
        break;
      case LATIN:
        // Переключение на режим латиницы
        mode = LATIN;
        continue;
      case CYRIL:
        // Переключение на режим кириллицы
        mode = CYRIL;
        continue;
      default:
        // Выводим символ, прогоняя его через переводчик
        fputc(decode(a, 0, last), output);
        last = a[0];
        break;
    }
  }
  // Закрываем файлы
  fclose(input);
  fclose(output);
  printf("successfull\nOutput: %s\n", outfilename);
}

char decode(int* i, char state, int last) {
  char buf = '?';
  switch (state) {
    case L2:
      // Обработка двойной последовательности
      if (i[1] == '`') {
        buf = '\xfa';
        // Если прошлый символ большой - делаем твердый знак большим
        if (isupper(last)) buf = toupcyril(buf);
        return buf;
      }
      switch (tolower(i[1])) {
        case 'z':
          // Буква 'ж'
          buf = '\xe6';
          break;
        case 'c':
          // Буква 'ч'
          buf = '\xf7';
          break;
        case 's':
          // Буква 'ш'
          buf = '\xf8';
          break;
        case 'y':
          // Буква 'я'
          if (i[2] == 'a')
            buf = '\xff';
          else if (i[2] == '`')
            // Буква 'ы'
            buf = '\xfb';
          else if (i[2] == 'o')
            // Буква 'ё'
            buf = '\xb8';
          else
            // Буква 'ю'
            buf = '\xfe';
          break;
        case 'e':
          // Буква 'э'
          buf = '\xfd';
          break;
        default:
          buf = '?';
          return buf;
      }
      // Если символ большой, делаем кириллический символ также большим
      if (isupper(i[1])) buf = toupcyril(buf);
      break;
    case L3:
      // Обработка тройной последовательности
      buf = '\xf9';  // Буква 'щ'
      // Если символ большой, делаем кириллический символ также большим
      if (isupper(i[1])) buf = toupcyril(buf);
      break;
    case 0:
    default:
      // Обработка обычных однобайтных символов
      if (isalpha(i[0])) {
        // Если это символ, переводим его по таблице
        buf = fromtrl[convto(i[0])];
        // Если символ большой, делаем кириллический символ также большим
        if (isupper(i[0])) buf = toupcyril(buf);
      } else if (i[0] == '`') {
        // Отдельно уделяем внимание мягкому знаку, он не буква :(
        buf = '\xfc';
        // Если прошлый символ большой - делаем мягкий знак большим
        if (isupper(last)) buf = toupcyril(buf);
      } else
        // Не букву оставляем без изменений
        buf = i[0];
      break;
  }
  return buf;
}