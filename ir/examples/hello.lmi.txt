%entry start #伪指令指定入口位置

start:
    newstr r3 "Hello World\0A\00" #return address to reg3
    vmcall 0 #write

    movri r3, 0 #
    vmcall 3 #exit
