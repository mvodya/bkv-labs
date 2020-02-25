// Operation systems
// Lab 1
// by Mark Vodyanitskiy

#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "partstypes.h"

// Файловый дескриптор
int fd;
// Буффер с началом файла (или диска)
unsigned char buffer[512];

// Читает раздел
void read_part(int n) {
  // Смешаем на 0x10 для каждого раздела
  int offset = 0x01be + (n * 0x10);
  // Указываем, откуда собираемся считать (смещение)
  lseek(fd, offset, SEEK_SET);
  // Читаем 16 байт
  read(fd, buffer, 0x10);

  printf("Partition #%d:\n", n + 1);
  // Является ли раздел загрузочным?
  if (buffer[0] == 0x80) {
    printf("Bootable\n");
  } else {
    printf("Not bootable\n");
  }

  // Определяем тип раздела
  printf("Type: ");
  print_type(buffer[0x4]);
  printf("\n");

  // Определяем стартовый сектор и размер раздела (в секторах)
  // немного черная магия ~
  unsigned int* sb = (unsigned int*)(buffer + 0x8);
  unsigned int* ns = (unsigned int*)(buffer + 0xC);

  printf("Start sector: %d / size: %d\n\n", *sb, *ns);
}

int main(int argc, char* argv[]) {
  // Требуем аргумент (имя файла / путь до диска)
  if (argc < 2) {
    printf("Err. No input filename.\n");
    return -1;
  }

  // Открываем декриптор в режиме ТОЛЬКО ЧТЕНИЕ
  fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    // Файл не найден
    printf("Err. File \"%s\" not found.\n", argv[1]);
    return -1;
  }
  
  // Читаем 4 раздела
  for(int i = 0; i < 4; i++)
  read_part(i);

  return 0;
}