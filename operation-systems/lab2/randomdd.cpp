// Operation systems
// Lab 2 A
// by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, ☜(ﾟヮﾟ☜)

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Размер сектора (в байтах)
#define SECTOR_SIZE 512

// Файловый дескриптор
int fd;

// Записываем рандомные символы в буффер
void randsector(char sector[], int c) {
  for (int i = 0; i < c; i++) {
    // С вероятностью 1/5 будет записано что-то рандомное
    if (!(rand() % 5))
      sector[i] = rand() % 0x100;
    else
      sector[i] = 0;
  }
}

int main(int argc, char *argv[]) {
  // Устанавливаем семя для рандомного генератора
  srand(1);

  // Требуем аргументы
  if (argc < 2) {
    printf("Use: randomdd <filename>\n");
    return -1;
  }

  // Открываем дескриптор в режиме ЧТЕНИЕ и ЗАПИСЬ
  fd = open(argv[1], O_RDWR);
  if (fd == -1) {
    // Файл не найден
    printf("Err. File \"%s\" not found.\n", argv[1]);
    return -1;
  }

  struct stat fdstat;
  // Получаем информацию о файле
  fstat(fd, &fdstat);
  // Вытаскиваем размер файла (в байтах) и переводим в кол-во секторов
  int size = fdstat.st_size / SECTOR_SIZE;

  // Проверка кратности размера и количества секторов
  if (fdstat.st_size % SECTOR_SIZE) {
    printf("Err. Not valid sector size.\n");
    return -1;
  }

  // Буффер рандомного сектора
  char sector[SECTOR_SIZE];

  // Перебираем все сектора
  for (int i = 0; i < size; i++) {
    // С вероятностью 1/2 заполняем сектор рандомом :3
    if (rand() % 2) {
      // Устанавливаем указатель на начало сектора
      lseek(fd, (i * SECTOR_SIZE), SEEK_SET);
      // Генерируем рандомный сектор
      randsector(sector, SECTOR_SIZE);
      // Блокировка перед записью
      lockf(fd, F_LOCK, SECTOR_SIZE);
      // Запись рандомного сектора
      write(fd, &sector, SECTOR_SIZE);
      // Снятие блокировки
      lockf(fd, F_ULOCK, SECTOR_SIZE);
    }
  }

  close(fd);
  return 0;
}