// Программа для перевода русского текста в транслит по ISO 9-95
// by Mark Vodyanitskiy (@mvodya) 15.05.2019
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define L2 '\x12'  // двухбуквенная последовательность
#define L3 '\x13'  // трехбуквенная последовательность
#define LATIN '\x14'  // режим латиницы
#define CYRIL '\x15'  // режим кириллицы

// Таблица перевода с кириллицы на латиницу
// cp-1251
const char *totrl[] = {
    "a",        // а - 192
    "b",        // б - 193
    "v",        // в - 194
    "g",        // г - 195
    "d",        // д - 196
    "e",        // е - 197
    "\x12zh",   // ж - 198
    "z",        // з - 199
    "i",        // и - 200
    "j",        // й - 201
    "k",        // к - 202
    "l",        // л - 203
    "m",        // м - 204
    "n",        // н - 205
    "o",        // о - 206
    "p",        // п - 207
    "r",        // р - 208
    "s",        // с - 209
    "t",        // т - 210
    "u",        // у - 211
    "f",        // ф - 212
    "x",        // х - 213
    "c",        // ц - 214
    "\022ch",   // ч - 215
    "\x12sh",   // ш - 216
    "\x13shh",  // щ - 217
    "\x12``",   // ъ - 218
    "\x12y`",   // ы - 219
    "`",        // ь - 220
    "\022e`",   // э - 221
    "\x12yu",   // ю - 222
    "\x12ya",   // я - 223
    "\x12yo"    // ё - 168 (184) учитывается отдельно
};

// Перевод файла на транслит
void translit(char *filename);
// Перевод буквы на последовательность в транслите
void encode(int i, char *buf, int last, char &mode);

int main(int argc, char **argv) {
  // Проверяем наличие аргументов
  if (argc == 1) {
    printf("You must specify paths of the files to be processed.\n");
    return 1;
  }

  // Перебираем аргументы
  while (--argc) translit(argv[argc]);

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
// Смещение кода кириллического символа для таблицы totrl
inline int convto(int i) {
  return ((i & (-17)) == 168) ? 32 : (i - (char)192) % (char)224;
}

void translit(char *filename) {
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
  int a, last = 0;
  char buf[5], mode = CYRIL;
  // Читаем посимвольно файл input до EOF
  while ((a = fgetc(input)) != EOF) {
    if (mode == LATIN && iscyrilic(a)) {
      // Если режим LATIN, и встретился кириллический символ, перключаем режим
      mode = CYRIL;
      fputc(CYRIL, output);
    }
    // Переводим символ в последовательность латинских символов
    encode(a, buf, last, mode);
    // Записываем последовательность в выходной файл
    fputs(buf, output);
    // Обновляем последний символ
    last = a;
  }
  fclose(input);
  fclose(output);
  printf("successfull\nOutput: %s\n", outfilename);
}

void encode(int i, char *buf, int last, char &mode) {
  if (iscyrilic(i)) {
    // Если символ кириллический, переводим его по таблице
    strcpy(buf, totrl[convto(i)]);
    if (isupcyril(i)) {
      // Если был большим, делаем первый переведенный символ таким же
      if (buf[0] == L2 || buf[0] == L3)
        // Т.к. при двойной или тройной последовательности добавляется
        // специальный символ флаг L2 или L3, то переводя первый символ
        // не забываем про смещение
        buf[1] = toupper(buf[1]);
      else
        buf[0] = toupper(buf[0]);
    }
    return;
  } else if (isalpha(i) && !isalpha(last) && mode == CYRIL) {
    // Если попадается первый латинский символ, добавляем флаг LATIN
    // и перключаем режим
    buf[0] = LATIN;
    buf[1] = i;
    buf[2] = '\0';
    mode = LATIN;
    return;
  } else {
    // Если это вообще не буква - молча добавляем
    buf[0] = i;
    buf[1] = '\0';
  }
}