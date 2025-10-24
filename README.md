# Lamina-VM 

## 目录
- [特性]
- [快速开始]
  ```bat
  mkdir out
  cd out
  cmake ..
  ninja 
  .\lmlc.exe ../test/hello.lmi hello.lmc
  .\lmvm.exe ./hello.lmc
  ```
- [架构设计]<br>
  采用了类Intel X86指令集<br>
  指令后缀，使用imm(i),mem(m),reg(r)来表示指令参数<br>
  分为双后缀（例如movrr,movrm）和单后缀指令（例如addr,addi）<br>
  全局64位寻址模式，有r0-r254的8字节寄存器和heap<br>
- [贡献]
- [许可证]

---

## 特性
- ✅ **高性能**：基于 C++ 高效内存操作
- ✅ **可扩展**：模块化指令集架构

---








