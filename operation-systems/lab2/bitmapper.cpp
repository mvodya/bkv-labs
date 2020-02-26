// Operation systems
// Lab 2 B
// by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, ¯\(°_o)/¯

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Размер сектора (в байтах)
#define SECTOR_SIZE 512

typedef long long int lli;

// Файловый дескриптор входного/выходного файла
int fdi, fdo;

int main(int argc, char *argv[]) {
  // Требуем два аргумента
  if (argc < 3) {
    printf("Use: bitmapper <input> <output_bitmap>\n");
    return -1;
  }

  // Открываем дескриптор входного файла в режиме ТОЛЬКО ЧТЕНИЕ
  fdi = open(argv[1], O_RDONLY);
  if (fdi == -1) {
    // Файл не найден
    printf("Err. File \"%s\" not found.\n", argv[1]);
    return -1;
  }

  // Открываем дескриптор выходного файла в режиме ТОЛЬКО ЗАПИСЬ
  fdo = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 644);
  if (fdo == -1) {
    // Файл не найден
    printf("Err. Can't open file \"%s\" for write.\n", argv[2]);
    return -1;
  }

  struct stat fdistat;
  // Получаем информацию о файле
  fstat(fdi, &fdistat);
  // Вытаскиваем размер файла (в байтах) и переводим в кол-во секторов
  int size = fdistat.st_size / SECTOR_SIZE;

  // Буффер сектора
  char sector[SECTOR_SIZE];

  // Битовая карта
  // В одном символе записано 8 секторов
  //                           + 1 символ, если не кратно 8
  int bitmap_size = (size / 8) + ((size % 8) > 0 ? 1 : 0);
  char *bitmap = new char[bitmap_size];
  // Устаналиваем нули в битовую карту
  for (int i = 0; i < bitmap_size; i++) bitmap[i] = 0;

  // Перебираем все сектора
  for (int i = 0; i < size; i++) {
    // Устанавливаем указатель на начало сектора
    lseek(fdi, (i * SECTOR_SIZE), SEEK_SET);
    // Читаем сектор в буффер
    read(fdi, sector, SECTOR_SIZE);

    // Проверка занятости сектора
    bool busy = false;
    for (int j = 0; j < SECTOR_SIZE; j += sizeof(lli)) {
      // Сравниваем сразу 8 (надеюсь) байт
      // поэтому охапкой сразу берем long long int
      lli *p = (lli *)(sector + j);
      if (*p != 0) {
        busy = true;
        break;
      }
    }

    // Устанавливаем бит если сектор занят
    if (busy) bitmap[i / 8] |= 0x80 >> (i % 8);
  }

  // Записываем битовую карту в выходной файл
  write(fdo, bitmap, bitmap_size);

  close(fdo);
  return 0;
}