org 100h

start:
in al, 0
out 0, al
push ax

and ax, 0FH ; clear ah, keep al
add ax, digits ; offset in array

mov si, ax
mov al, [si]
out 90H, al

pop ax
mov cx, 4
shr al, cl

and ax, 0FH ; clear ah, keep al
add ax, digits ; offset in array

mov si, ax
mov al, [si]
out 92H, al

jmp start

digits db 3FH, 6h, 5bh, 4fh, 66h, 6dh, 7dh, 7h, 7fh, 6fh, 77h, 7ch, 39h, 5eh, 79h, 71h, 0
;         0    1   2    3    4    5    6    7   8    9    A    B    C    D    E    F