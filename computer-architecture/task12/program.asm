; Task12 for computer architecture subject
; Alarm clock v2
; by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev

section .data
global hour, min, sec, hourz, minz, secz, hourA, minA, secA
hour db 0
min db 0
sec db 0
hourz db 0
minz db 0
secz db 0
hourA db 0
minA db 0
secA db 0

%include "rtc.asm"

section .text
global Alarm


; ============================================= ;
;           Подпрограмма БУДИЛЬНИК              ;
Alarm:
  push rax
  push rbx
  push rcx
  push rsi

  BLOCK_CYCLE_UPDATE

  RTC_GET 0, sec

  RTC_GET 2, min

  RTC_GET 4, hour

  ; Сложение секунд
  mov al, [sec] 
  add al, [secz]
  cmp al, 60     ; если больше 60, переносим 1
  jnae sum_sec_ok
  sub al, 60
  mov cx, [minz]
  inc cx
  mov [minz], cx

sum_sec_ok:
  BIN2BCD ax, al, ah
  mov [secA], al

  ; Сложение минут
  mov al, [min]
  add al, [minz]
  cmp al, 60     ; если больше 60, переносим 1
  jnae sum_min_ok
  sub al, 60
  mov cx, [hourz]
  inc cx
  mov [hourz], cx

sum_min_ok:
  BIN2BCD ax, al, ah
  mov [minA], al

  ; Сложение часов
  mov al, [hour]
  add al, [hourz]
  BIN2BCD ax, al, ah
  mov [hourA], al

  ; Запись секунд в будильник 
  WRITE_ALARM 1, secA

  ; Запись минут в будильник 
  WRITE_ALARM 3, minA

  ; Запись часов в будильник 
  WRITE_ALARM 5, hourA

  UNBLOCK_CYCLE_UPD

  RTC_ALARM_ON

  ; Ожидание срабатывания будильника
  WAITER
  ; Будильник сработал
  
  RTC_ALARM_OFF

  pop rsi
  pop rcx
  pop rbx
  pop rax
  ret
