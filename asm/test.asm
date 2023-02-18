

in 
pop ax

push ax
push 1024

jbe : 2

push 0
pop cx

push 0
pop bx

: 1

push [bx]
push 1
push cx   # cx ++
add
pop cx

push 100
pop [bx]     # mov [bx], 0064h

push bx
push 1     # bx++
add
pop bx

push cx
push ax
jne : 1

video
: 2
hlt