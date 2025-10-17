# LMVM - Lamina 虚拟机运行时 （弃用）

<!-- [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE) -->

一个用 Rust 实现的高效 **Lamina后端运行时**，深度绑定Rust高安全特性。

## 目录
- [特性]
- [快速开始]
  ```bat
  cmake
  .\lmvm.exe
  ```
- [架构设计]<br>
  采用了类Intel X86指令集，基于Rust语言实现。<br>
  借鉴了AT&T指令后缀，使用immu(i),memory(m),register(r)来表示指令参数<br>
  分为双后缀（例如movrr,movrm）和单后缀指令（例如addr,addi）<br>
  采用block块指令结构，指令集由指令块组成<br>
  支持部分高级语法，强类型（未实现）<br>
  不采用栈模型，全局64位寻址模式，有r0-r254的8字节寄存器和heap<br>
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


