
db "Factorial._Enter_the_number_^"

in
pop ax

db "\n^"

push 1
pop bx

push 1
pop fx

call 1

push fx

push ax
print 
out

db "!_=_^"

print

db "\n^"

hlt

: 1

push bx
push ax

je : 2

push fx
push 1
push bx
sum
pop bx
push bx
mul

pop fx

call 1

: 2

ret