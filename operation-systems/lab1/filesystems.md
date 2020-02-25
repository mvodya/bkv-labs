# Типы файловых систем

| hex | name            | hex | name            | hex | name            | hex | name               |
| --- | --------------- | --- | --------------- | --- | --------------- | --- | ------------------ |
| 0   | Empty           | 1b  | Hidden Win95 FA | 64  | Novell Netware  | bb  | Boot Wizard hid    |
| 1   | FAT12           | 1c  | Hidden Win95 FA | 65  | Novell Netware  | c1  | DRDOS/sec FAT      |
| 2   | XENIX root      | 1e  | Hidden Win95 FA | 70  | DiskSecure Mult | c4  | DRDOS/sec FAT      |
| 3   | XENIX usr       | 24  | NEC DOS         | 75  | PC/IX           | c6  | DRDOS/sec FAT      |
| 4   | FAT16 <32M      | 39  | Plan 9          | 80  | Old Minix       | c7  | Syrinx             |
| 5   | Extended        | 3c  | PartitionMagic  | 81  | Minix / old Lin | da  | Non-FS data        |
| 6   | FAT16           | 40  | Venix 80286     | 82  | Linux swap      | db  | CP/M / CTOS        |
| 7   | HPFS/NTFS       | 41  | PPC PReP Boot   | 83  | Linux           | de  | Dell Utility       |
| 8   | AIX             | 42  | SFS             | 84  | OS/2 hidden C:  | df  | BootIt             |
| 9   | AIX bootable    | 4d  | QNX4.x          | 85  | Linux extended  | e1  | DOS access         |
| a   | OS/2 Boot Manag | 4e  | QNX4.x 2nd part | 86  | NTFS volume set | e3  | DOS R/O            |
| b   | Win95 FAT32     | 4f  | QNX4.x 3rd part | 87  | NTFS volume set | e4  | SpeedStor          |
| c   | Win95 FAT32 (LB | 50  | OnTrack DM      | 8e  | Linux LVM       | eb  | BeOS fs            |
| e   | Win95 FAT16 (LB | 51  | OnTrack DM6 Aux | 93  | Amoeba          | ee  | EFI GPT            |
| f   | Win95 Ext'd (LB | 52  | CP/M            | 94  | Amoeba BBT      | ef  | EFI (FAT-12/16/32) |
| 10  | OPUS            | 53  | OnTrack DM6 Aux | 9f  | BSD/OS          | f1  | SpeedStor          |
| 11  | Hidden FAT12    | 54  | OnTrackDM6      | a0  | IBM Thinkpad hi | f4  | SpeedStor          |
| 12  | Compaq diagnost | 55  | EZ-Drive        | a5  | BSD/386         | f2  | DOS secondary      |
| 14  | Hidden FAT16 <3 | 56  | Golden Bow      | a6  | OpenBSD         | fd  | Linux raid auto    |
| 16  | Hidden FAT16    | 5c  | Priam Edisk     | a7  | NeXTSTEP        | fe  | LANstep            |
| 17  | Hidden HPFS/NTF | 61  | SpeedStor       | b7  | BSDI fs         | ff  | BBT                |
| 18  | AST SmartSleep  | 63  | GNU HURD or Sys | b8  | BSDI swap       |     |                    |