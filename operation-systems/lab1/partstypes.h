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
    case 0x64:
      printf("Novell Netware");
      break;
    case 0x65:
      printf("Novell Netware");
      break;
    case 0x70:
      printf("DiskSecure Mult");
      break;
    case 0x75:
      printf("PC/IX");
      break;
    case 0x80:
      printf("Old Minix");
      break;
    case 0x81:
      printf("Minix");
      break;
    case 0x82:
      printf("Linux swap");
      break;
    case 0x83:
      printf("Linux");
      break;
    case 0x84:
      printf("OS/2 hidden C:");
      break;
    case 0x85:
      printf("Linux extended");
      break;
    case 0x86:
      printf("NTFS volume set");
      break;
    case 0x87:
      printf("NTFS volume set");
      break;
    case 0x8e:
      printf("Linux LVM");
      break;
    case 0x93:
      printf("Amoeba");
      break;
    case 0x94:
      printf("Amoeba BBT");
      break;
    case 0x9f:
      printf("BSD/OS");
      break;
    case 0xa0:
      printf("IBM Thinkpad");
      break;
    case 0xa5:
      printf("BSD/386");
      break;
    case 0xa6:
      printf("OpenBSD");
      break;
    case 0xa7:
      printf("NeXTSTEP");
      break;
    case 0xb7:
      printf("BSDI fs");
      break;
    case 0xb8:
      printf("BSDI swap ");
      break;
    case 0xbb:
      printf("Boot Wizard hid");
      break;
    case 0xc1:
      printf("DRDOS/sec");
      break;
    case 0xc4:
      printf("DRDOS/sec");
      break;
    case 0xc6:
      printf("DRDOS/sec");
      break;
    case 0xc7:
      printf("Syrinx");
      break;
    case 0xda:
      printf("Non-FS data");
      break;
    case 0xdb:
      printf("CP/M / CTOS");
      break;
    case 0xde:
      printf("Dell Utility");
      break;
    case 0xdf:
      printf("BootIt");
      break;
    case 0xe1:
      printf("DOS access");
      break;
    case 0xe3:
      printf("DOS R/O");
      break;
    case 0xe4:
      printf("SpeedStor");
      break;
    case 0xeb:
      printf("BeOS fs");
      break;
    case 0xee:
      printf("EFI GPT");
      break;
    case 0xef:
      printf("EFI (FAT-12/16/32)");
      break;
    case 0xf1:
      printf("SpeedStor");
      break;
    case 0xf4:
      printf("SpeedStor");
      break;
    case 0xf2:
      printf("DOS secondary");
      break;
    case 0xfd:
      printf("Linux raid auto");
      break;
    case 0xfe:
      printf("LANstep");
      break;
    case 0xff:
      printf("BBT");
      break;
    default:
      printf("unknow %02x", type);
      break;
  }
}