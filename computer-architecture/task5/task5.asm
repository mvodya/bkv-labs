; Task5 for computer architecture subject
; Display digit using base
; by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova
org 100h

; base for number
base db 10


start:
; INPUT
in al, 0
out 0, al


mov cx, 8
mov dx, 90H ; Output led bar
l:
; N = al
; R = ah
div byte [base]
cmp ax, 0;
jz l2;
mov bl, ah
xor bh, bh
add bx, digits
push ax
mov al, [bx]
out dx, al ; output to led bar
pop ax
xor ah, ah ; clear remainder
; Offset led indicator
inc dx
inc dx
loop l
jmp start


; Space filler
l2:
cmp cx, 8
je zero
out dx, al ; output to led bar
; Offset led indicator
etp: inc dx
inc dx
loop l2
jmp start


; If input zero
zero:
mov al, 3FH
out dx, al
xor al, al
jmp etp


;         0    1   2    3    4    5    6    7   8    9    A    B    C    D    E    F
digits db 3FH, 6h, 5bh, 4fh, 66h, 6dh, 7dh, 7h, 7fh, 6fh, 77h, 7ch, 39h, 5eh, 79h, 71h