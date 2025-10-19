%entry start #伪指令指定入口位置

fib:
    pushr r3
    ble r3, fib.ret # if r3<=0 {ret r3}

    subi r3, 1
    call fib # fib(r3-1)
    popr r2
    push r0

    subi r2, 2
    movrr r3, r2
    call fib # fib(r3-2)
    popr r1
    addr r0, r1
    ret

fib.base:
    popr r0
    ret
    
start:
    movri r3, 30
    call fib
    # 返回数据在r3, vmcall参数也在r3，所不需要移动
    vmcall 2 #exit