                                        #This programm finds factorial 
                                        
db "Factorial. Enter the number: ^"

in
pop ax

db "\n^"

push 0
pop bx

push 1
pop fx

call 1

push fx

push ax
print 
out

db "! = ^"

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