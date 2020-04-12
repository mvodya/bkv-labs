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


; ============================================= ;
;           Подпрограмма БУДИЛЬНИК              ;
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
  call bin2bcd
  mov [secA], al

  ; Сложение минут
  mov al, [min]
  add al, [minz]
  call bin2bcd
  mov [minA], al

  ; Сложение часов
  mov al, [hour]
  add al, [hourz]
  call bin2bcd
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


; ============================================= ;
;  Функция безопасной блокировки обновления RTC ;
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


; ============================================= ;
;      Функция разблокировки обновления RTC     ;
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


; ============================================= ;
;     Перевод из BCD в обычный бинарный вид     ;
bcd2bin:
  push rbx
  push rcx
  push rdx

  mov bl, al
  and bl, 0Fh    ; masking
  and al, 0F0h   ; masking
  mov cl, 4
  ror byte al, cl; циклическое смещение вправо
  mov dl, 0Ah
  mul dl
  add al, bl
  
  ; al - ответ
  pop rdx
  pop rcx
  pop rbx
  ret


; ============================================= ;
;        Перевод из бинарного вида в BCD        ;
; алгоритм: https://my.eng.utah.edu/~nmcdonal/Tutorials/BCDTutorial/BCDConversion.html
bin2bcd:
    push rbx
    push rcx
    mov cx, 8
bin2bcd_loop:    ; перебор 8 раз
    rol ax, 1
    rol bx, 1
    ; если в ah есть 1, переносим в bl
    cmp ah, 0
    jnz bin2bcd_step
bin2bcd_full_continue:
    ; проверяем первые 4 бита bl, если >= 5, добавим 3
    push rbx
    and bl, 0Fh
    cmp bl, 4
    pop rbx
    ja bin2bcd_frap
    ; продолжение цикла
bin2bcd_continue:
    loop bin2bcd_loop
    ; конец перевода
    mov al, bl
    pop rcx
    pop rbx
    ret
    ; перенос бита в bl из ah
bin2bcd_step:
    xor ah, ah
    or bl, 1
    jmp bin2bcd_full_continue
    ; добавляем 3 для числа bl >= 5
bin2bcd_frap:
    cmp cx, 1    ; принудительный break
    je bin2bcd_continue
    inc bl
    inc bl
    inc bl
    jmp bin2bcd_continue