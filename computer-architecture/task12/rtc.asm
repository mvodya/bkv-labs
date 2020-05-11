; Task12 for computer architecture subject
; Alarm clock v2
; by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev

; ============================================= ;
;  Функция безопасной блокировки обновления RTC ;
%macro BLOCK_CYCLE_UPDATE 0
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
%endmacro

; ============================================= ;
;     Перевод из BCD в обычный бинарный вид     ;
%macro BCD2BIN 1
  push rbx
  push rcx
  push rdx

  mov bl, %1
  and bl, 0Fh    ; masking
  and %1, 0F0h   ; masking
  mov cl, 4
  ror byte %1, cl; циклическое смещение вправо
  mov dl, 0Ah
  mul dl
  add %1, bl

  ; %1 - ответ
  pop rdx
  pop rcx
  pop rbx
%endmacro

; ============================================= ;
;        Перевод из бинарного вида в BCD        ;
; алгоритм: https://my.eng.utah.edu/~nmcdonal/Tutorials/BCDTutorial/BCDConversion.html
%macro BIN2BCD 3
  push rbx
  push rcx
  mov cx, 8
%%bin2bcd_loop:    ; перебор 8 раз
    rol %1, 1
    rol bx, 1
    ; если в ah есть 1, переносим в bl
    cmp ah, 0
    jnz %%bin2bcd_step
%%bin2bcd_full_continue:
    ; проверяем первые 4 бита bl, если >= 5, добавим 3
    push rbx
    and bl, 0Fh
    cmp bl, 4
    pop rbx
    ja %%bin2bcd_frap
    ; продолжение цикла
%%bin2bcd_continue:
    loop %%bin2bcd_loop
    ; конец перевода
    mov %2, bl
    pop rcx
    pop rbx
    jmp %%bin2bcd_end
    ; перенос бита в bl из ah
%%bin2bcd_step:
    xor %3, %3
    or bl, 1
    jmp %%bin2bcd_full_continue
    ; добавляем 3 для числа bl >= 5
%%bin2bcd_frap:
    cmp cx, 1    ; принудительный break
    je %%bin2bcd_continue
    inc bl
    inc bl
    inc bl
    jmp %%bin2bcd_continue
%%bin2bcd_end:
%endmacro

; ============================================= ;
;      Функция разблокировки обновления RTC     ;
%macro UNBLOCK_CYCLE_UPD 0
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
%endmacro

; ============================================= ;
;          Запись времени в будильник 
%macro WRITE_ALARM 2
  mov dx, 70h    ; порт идекса RTC
  mov al, %1
  out dx, al     ; указываем регистр секунд/минут/часов будильника
  inc dx         ; переходим к порту данных
  mov al, [%2]
  out dx, al     ; записываем секунды/минуты/часы
%endmacro

; ============================================= ;
;       Ожидание срабатывания будильника
%macro WAITER 0
%%waiter_loop:
  mov dx, 70h    ; порт индекса RTC
  mov al, 0Ch
  out dx, al     ; указываем регистр C
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем регистр C
  and al, 20h    ; получаем 5-ый бит 
  je %%waiter_loop ; сравниваем с нулем
%endmacro

; ============================================= ;
;         Получение времени из RTC
%macro RTC_GET 2
  mov dx, 70h    ; порт индекса RTC
  mov al, %1
  out dx, al     ; указываем регистр
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем
  BCD2BIN al
  mov [%2], al   ; сохраним в переменную
%endmacro

; ============================================= ;
;            Включение будильника
%macro RTC_ALARM_ON 0
  mov dx, 70h    ; порт индекса RTC
  mov al, 0Bh
  out dx, al     ; указываем регистр B
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем регистр B
  or al, 20h     ; устанавливаем 5-ой бит
  out dx, al     ; записываем
%endmacro


; ============================================= ;
;           Выключение будильника
%macro RTC_ALARM_OFF 0
  mov dx, 70h    ; порт индекса RTC
  mov al, 0Bh
  out dx, al     ; указываем регистр B
  inc dx         ; переходим к порту данных
  in al, dx      ; прочитаем регистр B
  and al, 0DFh   ; устанавливаем 5-ой бит
  out dx, al     ; записываем
%endmacro