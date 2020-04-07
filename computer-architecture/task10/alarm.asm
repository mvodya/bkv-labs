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

  call block_cycle_upd

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

  ; Сложение секунд
  mov al, [sec] 
  add al, [secz] 
  mov [secA], al

  ; Сложение минут
  mov al, [min]
  add al, [minz]
  mov [minA], al

  ; Сложение часов
  mov al, [hour]
  add al, [hourz]
  mov [hourA], al

  pop rsi
  pop rcx
  pop rbx
  pop rax
  ret



block_cycle_upd:
  push rax
  push rdx

  mov dx, 71h    ; порт идекса RTC
bcu_loop:
  dec dx
  mov al, 0Bh    ; регистр B
  out dx, al     ; указываем регистр B
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем регистр B
  and al, 80h    ; получаем 7-ой бит 
  jne bcu_loop   ; сравниваем с нулем

  pop rdx
  pop rax
  ret