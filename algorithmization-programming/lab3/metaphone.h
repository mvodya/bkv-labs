// Кириллический Metaphone
// by Mark Vodyanitskiy (@mvodya) 30.05.2019
#pragma once
#include <ctype.h>
#include "utf8to1251.h"

inline char coversion(char i) {
  if (iscyrilic(i)) return tolowercyril(i);
  return tolower(i);
}

void metaphone(char* in, char* out) {
  char *p = in, *o = out;
  char c, n;
  while (*p != '\0') {
    c = coversion(*p);
    switch (c) {
      case '\xee':      // О
      case '\xfb':      // Ы
      case '\xff':      // Я
        *o++ = '\xe0';  // А
        break;
      case '\xe5':      // Е
      case '\xb8':      // Ё
      case '\xfd':      // Э
        *o++ = '\xe8';  // И
        break;
      case '\xfe':      // Ю
        *o++ = '\xf3';  // У
        break;
      case '\xe1':      // Б
        *o++ = '\xef';  // П
        break;
      case '\xe7':      // З
        *o++ = '\xf1';  // С
        break;
      case '\xe4':      // Д
        *o++ = '\xf2';  // Т
        break;
      case '\xe2':      // В
        *o++ = '\xf4';  // Ф
        break;
      case '\xe3':      // Г
        *o++ = '\xea';  // К
        break;
      // И или Й (двухбуквенная последовательность)
      case '\xe8':  // И
      case '\xe9':  // Й
        n = coversion(*(p + 1));
        // Следующая Е или О
        if (n == '\xe5' || n == '\xee') {
          *o++ = '\xe8';  // Сводится к И
          p++;
        } else
          *o++ = c;
        break;
      // Т (двухбуквенная последовательность)
      case '\xf2':  // Т
        n = coversion(*(p + 1));
        // Следующая С
        if (n == '\xf1') {
          *o++ = '\xf6';  // Сводится к Ц
          p++;
        } else
          *o++ = c;
        break;
      default:
        *o++ = c;
        break;
    }

    p++;
  }
  *o = '\0';
}