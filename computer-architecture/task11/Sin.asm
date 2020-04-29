; Task11 for computer architecture subject
; FPU Trinagle function drawer
; original by @bkv
; by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev

section .data
global HARM
global SINE
global calcul

MULTIP DW 200
RADIAN DW 180
REDUCE DW 360
ANGLE DW 0
TEMP1 DW 0
TEMP2 DW 0
TEMP3 DW 0
HARM DW 0

SINE times 361 DD 0           ; Массив вещественных чисел
ISINE times 361 DW 0          ; Массив целых чисел
saved times 100 dt 0.0

section .text
calcul:
  FSAVE [saved]
  push rax
  push rbx
  push rcx
  push rdx
  push rsi
  push rdi
  push rbp
  push rsp
  xor eax, eax
  mov rcx, 361
  mov rdi, SINE
  rep stosd
  mov word [ANGLE], 0

DOIT:
  MOV ESI, 0                  ; SI должен указывать на SINE

NXTPT:
  MOV word [TEMP1], 01H

ADMORE:
  MOV AX, [ANGLE]             ; Текущий угол в AX
  MOV DX, [TEMP1]
  SHL DX, 1                   ; Умн. на 2
  SUB DX, 1                   ; Вычесть 1
  PUSH AX
  PUSH DX
  MOV AX, DX
  MUL word DX
  POP DX
  MOV word [TEMP2], AX        ; Сохранить к-т гармоники k=1/(2*N-1)
  POP AX
  MUL word DX                 ; и умножить на угол. kwt.
  DIV word [REDUCE]
  MOV [TEMP3], DX
  FINIT                       ; Инициализация сопроцессора
  FILD word [RADIAN]
  FLDPI
  FDIV ST0, ST1               ; Получим что-то около .0174..
  FILD word [TEMP3]
  FMULP                       ; Переведем угол в радианы
  FCOS

POSSIG:
  FIDIV word [TEMP2]          ; Разделить на коэффициент гармоники

; Формула:
;         cos(wt) + 1/3 cos (3wt) + 1/5 cos (5wt) + ...

  FADD dword [SINE + ESI]
  FSTP dword [SINE + ESI]
  FWAIT                       ; Для целей синхронизации
  INC word [TEMP1]            ; Подготовимся сделать то же для высших гармоник
  MOV CX, [HARM]
  CMP [TEMP1], CX
  JG IDXPOS                   ; Переходим к следующему углу?
  JMP ADMORE

IDXPOS:
  ADD ESI, 04H
  INC word [ANGLE]            ; Переходим к следующему углу или
  CMP word [ANGLE], 360       ; Пора завершать работу?
  jg TRANS
  JMP NXTPT

TRANS:                        ; Завершение
  pop rsp
  pop rbp
  pop rdi
  pop rsi
  pop rdx
  pop rcx
  pop rbx
  pop rax
  FRSTOR [saved]
  ret
