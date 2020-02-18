// Реализация линейного списка
// by Mark Vodyanitskiy (@mvodya) 30.05.2019
#pragma once
#include <stdlib.h>
#include "person.h"

namespace list {

struct LIST {
  int id;          // Номер узла для поиска
  LIST* next;      // Указатель на следующий узел
  PERSON* person;  // Данные о пользователе
};

// Пустой ли список?
bool isEmpty(LIST* l) { return l == NULL; }

// Размер списка
int size(LIST* l) {
  int count = 0;
  LIST* c = l;
  while (c != NULL) {
    c = c->next;
    count++;
  }
  return count;
}

// Добавление нового элемента на верх списка
LIST* add(LIST* l, PERSON* p, int id) {
  LIST* n = (LIST*)malloc(sizeof(LIST));
  n->id = id;
  n->next = l;
  n->person = p;
  return n;
}

// Удаление головы списка
LIST* rmhead(LIST* l) {
  if (l == NULL) return NULL;
  LIST* n = l->next;
  free(l->person);
  free(l);
  return n;
}

// Поиск узла по номеру
LIST* searchID(LIST* l, int id) {
  LIST* s = l;
  while (s->next != NULL) {
    if (s->id == id) return s;
    s = s->next;
  }
  if (s->id == id) return s;
  return NULL;
}

// Удаление узла по номеру
LIST* removeID(LIST* l, int id) {
  LIST *s = l, *p = NULL, *a = l;
  while (s != NULL) {
    if (s->id == id) {
      if (s == l)
        a = l->next;
      else
        p->next = s->next;
      free(s->person);
      free(s);
      break;
    }
    p = s;
    s = s->next;
  }
  return a;
}

// Сортировка списка по имени
LIST* sortByName(LIST* l) {
  // Переводим линейный список в массив указателей на его элементы
  PERSON* arr[100000];
  LIST* p = l;
  int i = 0;
  while (p != NULL) {
    arr[i] = p->person;
    i++;
    p = p->next;
  }
  // Удаляем список
  LIST* n;
  p = l;
  do {
    n = p;
    p = p->next;
    free(n);
  } while (!isEmpty(p));
  // Сортируем
  qsort(arr, i, sizeof(PERSON*),
        (int (*)(const void*, const void*))cmpPERSONabc);
  // Переводим массив обратно в линейный список
  while (i--) {
    p = add(p, arr[i], i);
  }
  return p;
}

// Сортировка списка по возрасту
LIST* sortByAge(LIST* l) {
  // Переводим линейный список в массив указателей на его элементы
  PERSON* arr[100000];
  LIST* p = l;
  int i = 0;
  while (p != NULL) {
    arr[i] = p->person;
    i++;
    p = p->next;
  }
  // Удаляем список
  LIST* n;
  p = l;
  do {
    n = p;
    p = p->next;
    free(n);
  } while (!isEmpty(p));
  // Сортируем
  qsort(arr, i, sizeof(PERSON*),
        (int (*)(const void*, const void*))cmpPERSONage);
  // Переводим массив обратно в линейный список
  while (i--) {
    p = add(p, arr[i], i);
  }
  return p;
}

}  // namespace list