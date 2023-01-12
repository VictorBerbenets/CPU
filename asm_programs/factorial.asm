
text_begin

push 10
push 58
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
pop fx

call 1

push fx

push ax
print 
out

text_begin

push 32
push 61
push 32
push 33


text_end

print

text_begin

push 10

text_end

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