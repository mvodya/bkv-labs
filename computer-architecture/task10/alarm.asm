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
  mov al, 0
  out dx, al     ; указываем регистр секунд
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем секунды
  call bcd2bin
  mov [sec], al  ; сохраним секунды в переменную

  dec dx         ; порт идекса RTC
  mov al, 2
  out dx, al     ; указываем регистр минут
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем минуты
  call bcd2bin
  mov [min], al  ; сохраним минуты в переменную

  dec dx         ; порт идекса RTC
  mov al, 4
  out dx, al     ; указываем регистр часов
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем часы
  call bcd2bin
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

  ; Запись секунд в будильник 
  dec dx         ; порт идекса RTC
  mov al, 1
  out dx, al     ; указываем регистр секунд будильника
  inc dx         ; переходим к порту данных
  mov al, [secA]
  out dx, al     ; записываем секунды

  ; Запись минут в будильник 
  dec dx         ; порт идекса RTC
  mov al, 3
  out dx, al     ; указываем регистр минут будильника
  inc dx         ; переходим к порту данных
  mov al, [minA]
  out dx, al     ; записываем минуты

  ; Запись часов в будильник 
  dec dx         ; порт идекса RTC
  mov al, 5
  out dx, al     ; указываем регистр часов будильника
  inc dx         ; переходим к порту данных
  mov al, [hourA]
  out dx, al     ; записываем секунды

  call unblock_cycle_upd

  ; Включение будильника
  dec dx         ; порт идекса RTC
  mov al, 0Bh
  out dx, al     ; указываем регистр B
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем регистр B
  or al, 20h     ; устанавливаем 5-ой бит
  out dx, al     ; записываем

  ; Ожидание срабатывания будильника
waiter_loop:
  dec dx         ; порт идекса RTC
  mov al, 0Ch
  out dx, al     ; указываем регистр C
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем регистр C
  and al, 20h    ; получаем 5-ый бит 
  ;je waiter_loop ; сравниваем с нулем !!! ВРЕМЕННО ОТКЛЮЧЕНО

  ; Будильник сработал
  
  ; Выключение будильника
  dec dx         ; порт идекса RTC
  mov al, 0Bh
  out dx, al     ; указываем регистр B
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем регистр B
  and al, 0DFh   ; устанавливаем 5-ой бит
  out dx, al     ; записываем

  pop rsi
  pop rcx
  pop rbx
  pop rax
  ret



block_cycle_upd:
  push rax
  push rdx

  mov dx, 71h    ; порт данных RTC
  ; Ожидаем, пока RTC записывается
bcu_loop:
  dec dx         ; порт идекса RTC
  mov al, 0Ah
  out dx, al     ; указываем регистр A
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем регистр A
  and al, 80h    ; получаем 7-ой бит 
  jnz bcu_loop   ; сравниваем с нулем
  ; RTC свободен
  ; Блокируем запись у RTC
  dec dx         ; порт идекса RTC
  mov al, 0Bh
  out dx, al     ; указываем регистр B
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем регистр B
  or al, 80h     ; устанавливаем 7-ой бит
  out dx, al     ; записываем блокировку
  ; RTC заблокирован

  pop rdx
  pop rax
  ret



unblock_cycle_upd:
  push rax
  push rdx

  mov dx, 70h    ; порт индекса RTC
  mov al, 0Bh
  out dx, al     ; указываем регистр B
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем регистр B
  and al, 7Fh    ; устанавливаем 7-ой бит
  out dx, al     ; записываем 
  ; RTC разблокирован

  pop rdx
  pop rax
  ret



bcd2bin:
  push rbx
  push rcx
  push rdx

  mov bl, al

  ; masking
  and bl, 0Fh
  and al, 0F0h

  mov cl, 4
  ror byte al, cl ; циклическое смещение вправо
  mov dl, 0Ah
  mul dl
  add al, bl
  ; al - ответ

  pop rdx
  pop rcx
  pop rbx
  ret
  