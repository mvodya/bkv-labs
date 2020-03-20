; Task7 for computer architecture subject
; Fibonacci series
; by Mark Vodyanitskiy, Elena Bova

section .text
global _start

_start:
  mov eax, 1
  mov ebx, 1
  ; Print first two 1
  call print
  call print

fibbo_loop:
  mov ecx, eax          ; set first number
  add ecx, ebx          ; add second number
  jc exit               ; check overflow
  push eax
  mov eax, ecx
  ; Print number
  call print
  pop eax
  ; Swap numbers
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
  xor ecx, ecx          ; size (clear)
  inc ecx               ; for new line print

print_div_loop:         ; convert number to ascii char array
  xor edx, edx
  div esi
  add edx, 48           ; add ascii offset (for character '0')
  
  dec ebx
  mov [ebx], dl         ; reverse save
  inc ecx               ; size counter
  cmp eax, 0
  jnz print_div_loop

  ; Print number
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
str: times 21 db 0Ah    ; char array buffer for printing