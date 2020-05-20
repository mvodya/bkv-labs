; Task12 for computer architecture subject
; Alarm clock v3
; by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev

; Читаем регистр из RTC
; [RTC register], [destination]
%macro RTC_READ_REGISTER 2
  push rax
  mov al, %1
  out 70h, al  ; указываем регистр
  in al, 71h   ; прочитаем регистр
  mov %2, al
  pop rax
%endmacro



; Записывает в регистр RTC
; [RTC register], [write byte]
%macro RTC_WRITE_REGISTER 2
  push rax
  mov al, %1
  out 70h, al  ; указываем регистр
  mov al, %2
  out 71h, al  ; записываем 
  pop rax
%endmacro



; Блокирует обновление RTC
%macro RTC_BLOCK_CYCLE_UPDATE 0
  push rbx

%%rtc_check_loop:            ; проверяем занятость RTC

  RTC_READ_REGISTER 0Ah, bl  ; читаем регистр A
  and bl, 80h                ; получаем 7-ой бит 
  jnz %%rtc_check_loop         ; сравниваем с нулем
  ; RTC свободен, блокируем запись
  RTC_READ_REGISTER 0Bh, bl  ; читаем регистр B
  or bl, 80h                 ; устанавливаем 7-ой бит
  RTC_WRITE_REGISTER 0Bh, bl ; записываем блокировку

  pop rbx
%endmacro



; Разблокирует обновление RTC
%macro RTC_UNBLOCK_CYCLE_UPDATE 0
  push rbx

  RTC_READ_REGISTER 0Bh, bl  ; читаем регистр B
  and bl, 7Fh                ; устанавливаем 7-ой бит
  RTC_WRITE_REGISTER 0Bh, bl ; записываем разблокировку

  pop rbx
%endmacro



; Перевод из BCD в бинарный вид
; [byte / destination]
%macro RTC_BCD2BIN 1
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

  pop rdx
  pop rcx
  pop rbx
%endmacro



; Перевод из бинарного вида в BCD
; алгоритм: https://my.eng.utah.edu/~nmcdonal/Tutorials/BCDTutorial/BCDConversion.html
; [register], [low part register], [hight part register]
%macro RTC_BIN2BCD 3
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



; Чтение секунд/минут/часов из RTC
; Выполнять только после блокировки
; [write byte]
%macro RTC_READ_REAL_SEC 1
  RTC_READ_REGISTER 0, %1
%endmacro
%macro RTC_READ_REAL_MIN 1
  RTC_READ_REGISTER 2, %1
%endmacro
%macro RTC_READ_REAL_HOUR 1
  RTC_READ_REGISTER 4, %1
%endmacro



; Чтение времени из RTC
; [destination var hour], [destination var min], [destination var sec]
%macro RTC_READ_REAL_TIME 3
  push rbx
  RTC_READ_REAL_SEC bl
  mov al, bl
  RTC_BCD2BIN al
  mov [%3], al

  RTC_READ_REAL_MIN bl
  mov al, bl
  RTC_BCD2BIN al
  mov [%2], al

  RTC_READ_REAL_HOUR bl
  mov al, bl
  RTC_BCD2BIN al
  mov [%1], al
  pop rbx
%endmacro



; Запись времени в будильник 
; Выполнять только после блокировки
; [location var]
%macro RTC_ALARM_WRITE_SEC 1
  RTC_WRITE_REGISTER 1, [%1]
%endmacro
%macro RTC_ALARM_WRITE_MIN 1
  RTC_WRITE_REGISTER 3, [%1]
%endmacro
%macro RTC_ALARM_WRITE_HOUR 1
  RTC_WRITE_REGISTER 5, [%1]
%endmacro



; Включение будильника
%macro RTC_ALARM_ON 0
  RTC_READ_REGISTER 0Bh, bl
  or bl, 20h                 ; устанавливаем 5-ой бит
  RTC_WRITE_REGISTER 0Bh, bl
%endmacro



; Выключение будильника
%macro RTC_ALARM_OFF 0
  RTC_READ_REGISTER 0Bh, bl
  and bl, 0DFh                ; устанавливаем 5-ой бит
  RTC_WRITE_REGISTER 0Bh, bl
%endmacro



; Ожидание срабатывания будильника
%macro RTC_ALARM_WAITER 0
%%waiter_loop:
  RTC_READ_REGISTER 0Ch, bl
  and bl, 20h      ; получаем 5-ый бит
  je %%waiter_loop ; сравниваем с нулем
%endmacro
