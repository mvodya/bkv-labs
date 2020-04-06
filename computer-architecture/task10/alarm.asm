; Task9 for computer architecture subject
; Alarm clock
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

section .text
global Alarm

Alarm:
  push rax
  push rbx
  push rcx
  push rsi

  ; TODO
  mov dx, 70h    ; порт идекса RTC
  mov al, 0      ; регистр секунд
  out dx, al     ; указываем регистр секунд
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем секунды
  mov [sec], al  ; сохраним секунды в переменную

  dec dx         ; порт идекса RTC
  mov al, 2      ; регистр минут
  out dx, al     ; указываем регистр минут
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем минуты
  mov [min], al  ; сохраним минуты в переменную

  dec dx         ; порт идекса RTC
  mov al, 4      ; регистр часов
  out dx, al     ; указываем регистр часов
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем часы
  mov [hour], al ; сохраним часы в переменную

  pop rsi
  pop rcx
  pop rbx
  pop rax
  ret