
db "Enter_coefficients_of_quadratic_equation\n^"


in 
pop ax
in
pop bx
in
pop cx

push ax
push 0

jne : 10

push 0
push bx
jne : 5

push 0
push cx

jne : 3

# Infinite Roots

db "Infinite_roots\n^" 

hlt

: 3

# NO ROOTS

db "No_Roots\n^"

hlt

# if bx != 0
: 5

push bx
push cx
push -1
mul
div
print

hlt

: 10

# If ax != 0

push 4
push ax
push cx
mul
mul

push bx
push bx
mul

sub

sqrt
pop dx

push dx
push 0

je : 20


push dx
push 0
jb : 25

# if dx < 0, end programm and print 'No roots'

db "discriminant_<_0_._^"

call 3    
hlt

: 25

# first Root

push 2   
push ax
mul
pop fx
push fx

push bx
push -1
mul

push dx

sum
div

db "first_root_^"

print

db "___^"

# second Root

push fx     

push dx

push bx
push -1
mul

sub

div

db "second_root_^"

print

db "\n^"

hlt

: 20

push 2
push ax
mul

push -1
push bx
mul
div

db "discriminant_=_0._The_root_is_^"

print

db "\n^"

meow

hlt




