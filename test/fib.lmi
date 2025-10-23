%entry start

fib:
    ble r3, fibret         # if r3 <= 0, return r3

    pushr r3              # 保存当前n
    subi r3, 1            # n-1
    call fib              # fib(n-1) -> r0
    movrr r1, r0          # 将结果暂存到r1
    
    popr r3               # 恢复原始n
    pushr r1              # 保存fib(n-1)结果
    
    subi r3, 2            # n-2
    call fib              # fib(n-2) -> r0
    popr r3               # 恢复fib(n-1)结果

    addr r0, r3           # r0 = fib(n-1) + fib(n-2)
    ret

fibret:
    movrr r0, r3          # 基准情况: return n (0或1)
    ret

start:
    movri r3, 30          # 计算fib(30)
    call fib
    print r0
