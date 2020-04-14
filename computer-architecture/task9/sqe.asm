; Task9 for computer architecture subject
; Calculate quadratic equation
; by Mark Vodyanitskiy, Elena Bova, Danil Maltsev
section .data
global root1
global root2
global a
global b
global c
global d
a dq 0.0
b dq 0.0
c dq 0.0
d dq 0.0
message db 'No roots!',10
four dq 4.0
two dq 2.0
root1 dq 0.0
root2 dq 0.0
temp2 times 256 dd 0    ; for saving fpu stack
global sq_eq


section .text
sq_eq:
   ; Calculate D
   finit
   fld qword [b]
   fmul qword [b]       ; b ^ 2
   fld qword [a]
   fld qword [four]
   fmul                 ; a * 4
   fld qword [c]
   fmul                 ; (a * 4) * c
   fsubp st1, st0       ; (b ^ 2) - (a * 4) * c
   fst qword [d]        ; load st0 to d

   ; D branching
   ftst                 ; compare with 0
   fstsw ax             ; get compare result
   sahf                 ; load ax to flags
   push qword ret_sq_eq ; save for return
   jc sq_eq_no_roots    ; less than zero - no roots
   jz sq_eq_one_root    ; equal zero - one root
   jmp sq_eq_two_roots  ; greater then zero - two roots
   ret_sq_eq:           ; exit for all functions

   ret

sq_eq_two_roots: 
   ; Calculate two roots
   finit
   fld qword [two]
   fmul qword [a]       ; 2 * a 
   fld qword [d] 
   fsqrt                ; sqrt(D)
   fld qword [b]
   fchs                 ; -b
   fsave [temp2+4]      ; save -b, d and 2*a
   frstor[temp2+4] 
   fsubr                ; -b - sqrt(d)
   fdivr                ; (-b - sqrt(d)) / (2 * a)
   fst qword [root1]    ; load st0 to root1
   frstor [temp2+4]
   fadd                 ; -b + sqrt(d)
   fdivr                ; (-b + sqrt(d)) / (2 * a)
   fst qword [root2]    ; load st0 to root2
   ret

sq_eq_no_roots: 
    ; No roots message
    push rax
    push rdi
    push rsi
    push rdx

    mov rax, 1          ; sys_write
    mov rdi, 1          ; stdout
    mov rsi, message
    mov rdx, 10         ; message size
    syscall             ; call sys_write

    pop rdx
    pop rsi
    pop rdi
    pop rax
    ret

sq_eq_one_root:
    ; Calculate one root
    finit
    fld qword [b]
    fchs                ; -b
    fld qword [two]
    fmul qword [a]      ; 2 * a
    fdiv                ; -b / (2 * a)
    fst qword [root1]
    ret


section .stack
times 256  dd 0
