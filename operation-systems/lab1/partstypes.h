// Разбор типа файловой системы

#include <stdio.h>

void print_type(unsigned char type) {
  switch (type) {
    case 0x0:
      printf("Unused");
      break;
    case 0x1:
      printf("FAT12");
      break;
    case 0x2:
      printf("XENIX root");
      break;
    case 0x3:
      printf("XENIX usr");
      break;
    case 0x4:
      printf("FAT16 <32M");
      break;
    case 0x5:
      printf("Extended");
      break;
    case 0x6:
      printf("FAT16");
      break;
    case 0x7:
      printf("Windows NT");
      break;
    case 0x8:
      printf("AIX");
      break;
    case 0x9:
      printf("AIX bootable");
      break;
    case 0x82:
      printf("Linux swap");
      break;
    case 0x83:
      printf("Linux");
      break;
    default:
      printf("unknow %02x", type);
      break;
  }
}