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
    case 0xa:
      printf("OS/2 Boot Manager");
      break;
    case 0xb:
      printf("Win95 FAT32");
      break;
    case 0xc:
      printf("Win95 FAT32");
      break;
    case 0xe:
      printf("Win95 FAT16");
      break;
    case 0xf:
      printf("Win95 Ext'd");
      break;
    case 0x10:
      printf("OPUS");
      break;
    case 0x11:
      printf("Hidden FAT12");
      break;
    case 0x12:
      printf("Compaq diagnostic");
      break;
    case 0x14:
      printf("Hidden FAT16");
      break;
    case 0x16:
      printf("Hidden FAT16");
      break;
    case 0x17:
      printf("Hidden HPFS/NTF");
      break;
    case 0x18:
      printf("AST SmartSleep");
      break;
    case 0x1b:
      printf("Hidden Win95 FA");
      break;
    case 0x1c:
      printf("Hidden Win95 FA");
      break;
    case 0x1e:
      printf("Hidden Win95");
      break;
    case 0x24:
      printf("NEC DOS");
      break;
    case 0x39:
      printf("Plan 9");
      break;
    case 0x3c:
      printf("PartitionMagic");
      break;
    case 0x40:
      printf("Venix 80286");
      break;
    case 0x41:
      printf("PPC PReP Boot");
      break;
    case 0x42:
      printf("SFS");
      break;
    case 0x4d:
      printf("QNX4.x");
      break;
    case 0x4e:
      printf("QNX4.x 2nd part");
      break;
    case 0x4f:
      printf("QNX4.x 3rd part");
      break;
    case 0x50:
      printf("OnTrack DM");
      break;
    case 0x51:
      printf("OnTrack DM6 Aux");
      break;
    case 0x52:
      printf("CP/M");
      break;
    case 0x53:
      printf("OnTrack DM6 Aux");
      break;
    case 0x54:
      printf("OnTrackDM6");
      break;
    case 0x55:
      printf("EZ-Drive");
      break;
    case 0x56:
      printf("Golden Bow");
      break;
    case 0x5c:
      printf("Priam Edisk");
      break;
    case 0x61:
      printf("SpeedStor");
      break;
    case 0x63:
      printf("GNU HURD or Sys");
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