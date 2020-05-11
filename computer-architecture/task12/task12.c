// Task12 for computer architecture subject
// Alarm clock with macros
// by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev
#include <stdio.h>
#include <sys/io.h>

extern unsigned char hour, min, sec, hourz, minz, secz;
extern unsigned char hourA, minA, secA;

void Alarm(void);

int main()
{
  if(ioperm (0x70, 2, 1)) {
    printf("Can't open RTC port\nPlease, run this as root\n");
    return -1;
  }
  
  scanf("%x%x%x", &hourz, &minz, &secz);
  printf("Вы ввели      [HEX]: %02x:%02x:%02x\n", hourz, minz, secz);
  fflush(stdout);

  Alarm ();
  printf ("ALARM!!!\n");
  printf ("Текущее время [HEX]: %02x:%02x:%02x\n",hour,min,sec);
  printf ("Будильник     [BCD]: %02x:%02x:%02x\n",hourA,minA,secA);
  ioperm(0x70, 2, 0);
  
  return 0;
}
