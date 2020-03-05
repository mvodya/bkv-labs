; Task4 for computer architecture subject
; Fibonacci series
; by Mark Vodyanitskiy
org 100h

mov bx, store
loop: mov ax, word [bx]
add ax, word [bx + 2]   ; Summation
jc end                  ; Overflow check
mov word [bx + 4], ax
add bx, 2d              ; Offset   
jmp loop
end: jmp end

; First 1 for starting
store: dw 1d, 1d