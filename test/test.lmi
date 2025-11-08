%entry id

id:
newi r4 332323
movrm r3,[r4+0]
movri r9, 100
loop:


    print r3
print r1
addi r1,1
cmp r1,r9
jle loop

