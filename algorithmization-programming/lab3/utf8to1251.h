// Перевод из cp1251 <-> UTF-8
// by Mark Vodyanitskiy (@mvodya) 30.05.2019
#pragma once
#include <iconv.h>

// Перевод из UTF-8 в CP1251
void utf8to1251(char* in, char* out) {
  iconv_t i = iconv_open("CP1251", "UTF-8");
  size_t len = 256;
  iconv(i, &in, &len, &out, &len);
  iconv_close(i);
}

// Перевод из CP1251 в UTF-8
void cp1251toUtf8(char* in, char* out) {
  iconv_t i = iconv_open("UTF-8", "CP1251");
  size_t len = 256;
  iconv(i, &in, &len, &out, &len);
  iconv_close(i);
}

// Является ли символ кириллической буквой?
inline char iscyrilic(int i) {
  // Учитывается буква Ё (аналог tolower - побитовое уножение на -17)
  if ((i >= -64 && i <= -32) || (i == -88)) return 1;
  return 0;
}
// Перевод кириллической буквы в маленькую (с учетом Ё)
inline char tolowercyril(char i) { return (i == '\xa8') ? '\xb8' : (i | 32); }

// Сравнение кириллических букв
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
};