; Task12 for computer architecture subject
; Alarm clock v3
; by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev

section .data

global hourIN, minIN, secIN
hourIN db 0
minIN db 0
secIN db 0

global hourREAL, minREAL, secREAL
hourREAL db 0
minREAL db 0
secREAL db 0

global hourALARM, minALARM, secALARM
hourALARM db 0
minALARM db 0
secALARM db 0

%include "rtclib.asm"

section .text
global alarm

alarm:
  push rax
  push rbx
  push rcx

  RTC_BLOCK_CYCLE_UPDATE

  RTC_READ_REAL_TIME hourREAL, minREAL, secREAL

  ; Сложение секунд
  mov al, [secREAL]
  add al, [secIN]
  cmp al, 60
  jnae sum_sec_ok   ; если больше 60 нужно добавить минуту
  sub al, 60        ; отнимаем 60
  mov cx, [minIN]
  inc cx
  mov [minIN], cx   ; добавляем минуту

  sum_sec_ok:
  RTC_BIN2BCD ax, al, ah
  mov [secALARM], al

  ; Сложение минут
  mov al, [minREAL]
  add al, [minIN]
  cmp al, 60
  jnae sum_min_ok   ; если больше 60 нужно добавить час
  sub al, 60        ; отнимаем 60
  mov cx, [hourIN]
  inc cx
  mov [hourIN], cx  ; добавляем час

  sum_min_ok:
  RTC_BIN2BCD ax, al, ah
  mov [minALARM], al


  ; Сложение часов
  mov al, [hourREAL]
  add al, [hourIN]
  RTC_BIN2BCD ax, al, ah
  mov [hourALARM], al

  ; Запись в будильник 
  RTC_ALARM_WRITE_SEC secALARM
  RTC_ALARM_WRITE_MIN minALARM
  RTC_ALARM_WRITE_HOUR hourALARM

  RTC_UNBLOCK_CYCLE_UPDATE

  RTC_ALARM_ON
  RTC_ALARM_WAITER
  RTC_ALARM_OFF

  pop rcx
  pop rbx
  pop rax
