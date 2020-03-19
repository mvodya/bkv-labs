; Task7 for computer architecture subject
; Fibonacci series
; by Mark Vodyanitskiy

; WORK IN PROGRESS

section .text
global _start

_start:
  mov eax, 1
  mov ebx, 1
  ; print first two 1
  call print
  call print

fibbo_loop:
  mov ecx, eax
  add ecx, ebx
  jc exit
  push eax
  mov eax, ecx
  call print
  pop eax
  mov eax, ebx
  mov ebx, ecx
  jmp fibbo_loop

exit:
  ; Program exit (return 0)
  mov eax, 1
  mov ebx, 0
  int 80h


print:
  push eax
  push ebx
  push ecx
  push edx
  push esi

  mov esi, 10
  mov ebx, str + 20
  xor ecx, ecx ; size (clear)
  inc ecx ; for new line print

print_div_loop:
  xor edx, edx
  div esi
  add edx, 48 ; add ascii offset (for character '0')
  
  dec ebx
  mov [ebx], dl ; reverse save
  inc ecx ; size counter
  cmp eax, 0
  jnz print_div_loop

  ; print number
  mov edx, ecx
  mov ecx, ebx
  mov ebx, 1
  mov eax, 4
  int 80h

  pop esi
  pop edx
  pop ecx
  pop ebx
  pop eax

  ret


section .data
str: times 21 db 0Ah