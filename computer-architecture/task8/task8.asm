; Task8 for computer architecture subject
; Hofstadter sequence
; by Mark Vodyanitskiy, Elena Bova

; Q(n) = Q(n - Q(n - 1)) + Q(n - Q(n - 2)), n > 2
;                  B                 A

; 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15  16  17  18  19
; 1  1  2  3  3  4  5  5  6  6  6  8  8  8  10  9  10  11  11  12


section .text
global qnumbers
extern digits

qnumbers:
  mov rsi, dword 2
qloop:

  mov rax, rsi
  call qcalc
  mov rcx, rax
  mov rax, rsi
  call qgetpointer
  mov [rax], rcx  

  inc rsi
  cmp rsi, 3000040
  jne qloop

  ret

qcalc:
  ; input: rax - n
  ; output rax - Q(n)
  ; Q(n) = Q(n - Q(n - 1)) + Q(n - Q(n - 2)), n > 2
  ;                  B                 A
  push rsi
  push rcx
  mov rsi, rax

  ; PART B
  mov rcx, rsi 
  dec rax
  call qgetpointer
  sub rcx, [rax]
  mov rax, rcx
  call qgetpointer
  mov rcx, [rax]
  push rcx
  

  ; PART A
  mov rcx, rsi
  mov rax, rsi
  dec rax
  dec rax
  call qgetpointer
  sub rcx, [rax]
  mov rax, rcx
  call qgetpointer
  mov rcx, [rax]

  ; Summation B + A
  pop rax ; get rcx
  add rax, rcx

  pop rcx
  pop rsi
  ret

qgetpointer:
  ; input: rax - n
  ; output: rax - pointer to digits
  push rsi
  push rbx
  mov rsi, digits
  mov rbx, dword 8
  mul rbx
  add rax, rsi

  pop rbx
  pop rsi
  ret

section .data
;s: dq 1, 1
;digits: times 3000040 dq 0
;nums for calculating and output
