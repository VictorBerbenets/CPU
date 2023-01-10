
# the name of the programm

text_begin

push 10
push 58
push 99
push 32
push 98
push 32
push 97
push 32
push 114
push 101
push 116
push 110
push 69
push 10
push 110
push 111
push 105
push 116
push 97
push 117
push 113
push 101
push 32
push 99
push 105
push 116
push 97
push 114
push 100
push 97
push 117
push 81
push 10

text_end

# enter a b c

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

text_begin 

push 83
push 84
push 79
push 79
push 82
push 32
push 69
push 84
push 73
push 78
push 73
push 70
push 78
push 73

text_end 

hlt

: 3

# NO ROOTS

text_begin 

push 83
push 84
push 79
push 79
push 82
push 32
push 79
push 78

text_end 

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

# if dx < , end programm and print 'No roots'

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

print

# second Root

push fx     

push dx

push bx
push -1
mul

sub

div
print

hlt

: 20

push 2
push ax
mul

push -1
push bx
mul
div

print

meow

hlt




