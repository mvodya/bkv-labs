; Task6 for computer architecture subject
; Ticker
; by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev
org 100h

z: xor cx, cx ; clear offset
; main loop (with words offset)
l: cmp cx, 21d ; check end of word sequence
je z 

mov dx, 9EH ; last display element
mov si, msg
add si, cx ; add offset
push cx
mov cx, 8
; output all letters
printl: mov ax, [si]
out dx, al
inc si
; offset led display element
dec dx
dec dx
loop printl
pop cx

; WAITER ;
push cx
mov cx, 100
a1: nop
push cx
mov cx, 1000
a2: nop
loop a2
pop cx
loop a1
pop cx
; END WAITER ;

inc cx
jmp l

msg db 0, 0, 0, 0, 0, 0, 0, 0, 7ch, 6eh, 79h, 40h, 7ch, 6eh, 0f9h, 0, 7fh, 3fh, 7fh, 7dh, 82h, 0, 0, 0, 0, 0, 0, 0, 0
;                               B    Y    E    -    B    Y    E.       8    0    8    6    !