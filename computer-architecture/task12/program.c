#include <stdio.h>
#include <sys/io.h>

extern unsigned char hourIN, minIN, secIN;
extern unsigned char hourREAL, minREAL, secREAL;
extern unsigned char hourALARM, minALARM, secALARM;

void Alarm(void);

int main() {
  if(ioperm (0x70, 2, 1)) {
    printf("Can't open RTC port\nPlease, run this as root\n");
    return -1;
  }
  
  printf("Введите время в формате hh mm ss (DEC): ");
  scanf("%02d %02d %02d", &hourIN, &minIN, &secIN);
  printf("Вы ввели: %02d:%02d:%02d\n", hourIN, minIN, secIN);
  fflush(stdout);

  alarm ();
  printf ("ALARM\n");
  
  printf ("Время запуска: %02d:%02d:%02d\n", hourREAL, minREAL, secREAL);
  printf ("Будильник сработал в: %02x:%02x:%02x\n", hourALARM, minALARM, secALARM);

  ioperm(0x70, 2, 0); 
  return 0;
}
