# LMVM - Lamina 虚拟机运行时 （弃用）

<!-- [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE) -->

一个用 Rust 实现的高效 **Lamina后端运行时**，深度绑定Rust高安全特性。

## 目录
- [特性]
- [快速开始]
  ```bat
  git clone https://github.com/lamina-dev/LMVM.git
  cd LMVM
  cargo build --release
  .\target\release\LMVM.exe <你的二进制文件路径>
  ```
- [架构设计]<br>
  采用了类Intel X86指令集，基于Rust语言实现。<br>
  借鉴了AT&T指令后缀，使用immu(i),memory(m),register(r)来表示指令参数<br>
  分为双后缀（例如movrr,movrm）和单后缀指令（例如addr,addi）<br>
  采用block块指令结构，指令集由指令块组成<br>
  支持部分高级语法，强类型（未实现）<br>
  不采用栈模型，全局64位寻址模式，有r0-r9的8字节寄存器和heap<br>
- [指令集]<br>
  lmir的结构以block为主，block可以是块，方法等等，不属于任何block的指令会被立刻解释执行<br>
  - [基础指令]
     （学习lmir最好要有intel x86指令集基础,学起来会非常轻松）<br>
      (lmir主要的重点就是三种指令后缀:i,m,r。分别对应立即数，堆地址，寄存器)<br>
      ```asm
      movrr r0,r1     #r0 = r1
      movri r0,0x1000 #r0 = 0x1000
      movrm r0,[0x1000] #r0 = [0x1000]
      movmi [r0],12   #[r0] = 12
      movmr [r0],r1   #[r0] = r1
      movmm [r0],[r1] #[r0] = [r1]
      #六种mov指令，代码中0x1000只是示例，vm不会允许用户直接操作内存
      ```
      四则运算都遵循一种硬要求，左操作数必须为寄存器存储的一个地址，右操作数随意，因此只需要一个指令后缀
      ```asm
      addr r0,r1        #[r0] += r1
      addi r0,0x1000    #[r0] += 0x1000
      addm r0,[0x1000]  #[r0] += [0x1000]
      subr r0,r1        #[r0] -= r1 
      #不一一列举了，其他同理
      ```
  - [控制流指令]<br>
    这是lmir控制流的根基，block，它既是一个块，也是方法，可递归
    ```asm
    block add_two_i64
        movrr r0,r1
        addr  r0,r2
        ret
    end block add_two_i64


    movri r1,100
    movri r2,200
    call add_two_i64
    ```
    同样，支持块内部嵌套块，嵌套的块不存在独立的ret指令，ret指令属于大父块，不建议多层嵌套
    ```asm
    block the_block
    call .son_block
    cmpri r1, 100, ==
    if_true .the_block_ret
    if_false .the_block

      block .son_block
        movri r1,100
      end block .son_block

      block .the_block_ret
        ret
      end block .the_block_ret
    end block the_block
    ```
    再包含部分汇编指令的同时，我们同样支持部分高级语言的语法
    ```asm
    cmprr [r0],[r1],<=          #对于r0和r1目前指向的堆内存地址进行取值后进行比较,第三位为比较符号（==,!=,<,<=,>,>=）
    #这个示例就是 r0 <= r1 ? call the_cmp_is_true : call the_cmp_is_false
    if_true the_cmp_is_true   #如果比较结果为真，则跳转到the_cmp_is_true块
    if_false the_cmp_is_false #如果比较结果为假，则跳转到the_cmp_is_false块

    loop the_loop #一直执行the_loop块，直到遇到ret指令
    ```
- [贡献]
- [许可证]

---

## 特性
- ✅ **高性能**：基于 Rust 零成本抽象设计
- ✅ **安全内存**：所有权模型防止内存泄漏
- ✅ **可扩展**：模块化指令集架构

---

## 快速开始


### 安装

