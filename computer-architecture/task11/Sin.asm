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
TEMP2 DW -1
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
  mov word [TEMP2], -1

DOIT:
  MOV ESI, 0                  ; SI должен указывать на SINE

NXTPT:
  MOV word [TEMP1], 01H

ADMORE:
  MOV AX, word [TEMP2]        ; изменяем знак для чередования
  NEG word AX
  MOV word [TEMP2], AX
  MOV AX, [ANGLE]             ; Текущий угол в AX
  MUL word [TEMP1]            ; и умножить на угол. kwt.
  DIV word [REDUCE]
  MOV [TEMP3], DX
  FINIT                       ; Инициализация сопроцессора
  FILD word [RADIAN]
  FLDPI
  FDIV ST0, ST1               ; Получим что-то около .0174..
  FILD word [TEMP3]
  FMULP                       ; Переведем угол в радианы
  FSIN                        ; sin

POSSIG:
  FIDIV word [TEMP1]          ; Разделить на коэффициент гармоники
  
; Формула:
;         cos(wt) + 1/3 cos (3wt) + 1/5 cos (5wt) + ... (для треугольника)
;         sin(wt) - 1/2 sin (2wt) + 1/3 sin(3wt) - ... (Для пилы)
  
  mov BX, word [HARM]
  test word BX, 1             ; при четных и нечетных получается по-разному
  jne s1                      ; проверка на четность
  FIMUL word [TEMP2]
  FADD dword [SINE + ESI]
  jmp s2                      ; Переходим если число четное
  
s1:
  FSUB dword [SINE + ESI]
  
s2:
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
