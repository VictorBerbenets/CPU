text_begin

push 10
push 108
push 97
push 105
push 114
push 111
push 116
push 99
push 97
push 70
push 10


text_end

in
pop ax
push 1
pop bx
push 1
pop cx
push bx
: 1
push ax
je : 2
push 1
push bx
sum
pop bx
push cx
push bx
mul
pop cx 
push bx
jmp : 1
: 2
push cx

print
hlt