# Lamina-VM

> 此倉庫復刻自 Lamina-dev/LMVM
> 復刻者: AD

<div align="right">
    <a href="LMVM_README.md"> 原 Lamina-dev/LMVM 的 README.md </a>
</div>
<br>

###### 前言
```text
本倉庫復刻自 Lamina-dev/LMVM
本倉庫會定時和 Lamina-dev/LMVM 同步
如果需要提交更新可以自己去復刻或者聯繫 LMVM 當前倉庫維護者
```

## 目錄
- [快速開始 - 在 Windows 上](#快速開始---在-windows-上)
- [快速開始 - 在 iOS 上](#快速開始---在-ios-上)
- [快速開始 - 在 UNIX/Linux/MacOS 上](#快速開始---在UNIX-Linux-MacOS上)
- [架構設計](#架構設計)
<!-- - [貢獻](#貢獻) -->
- [測試人員](#測試人員)
<!-- - [許可證](#許可證) -->

## 快速開始 - 在 Windows 上
```bat
git clone https://github.com/Lamina-dev/LMVM.git
cd LMVM
mkdir build
cd build
cmake ..
cmake --build .
.\lmlc.exe ../../test/hello.lmi hello.lmc
.\lmvm.exe ./hello.lmc
```

## 快速開始 - 在 iOS 上
```bash
git clone https://github.com/Lamina-dev/LMVM.git
cd LMVM
mkdir -p build
cd build
cmake ..
make
ldid -S../entitlements/entitlements.plist bin/*
./lmlc ../../test/hello.lmi hello.lmc
./lmvm hello.lmc
```

## 快速開始 - 在 UNIX/Linux/MacOS 上
```bash
git clone https://github.com/Lamina-dev/LMVM.git
cd LMVM
mkdir -p build
cd build
cmake ..
make
./lmlc ../../test/hello.lmi hello.lmc
./lmvm hello.lmc

## 架構設計
採用了 ``類Intel X86指令集`` 指令後綴，使用 ``imm(i),mem(m),reg(r)`` 來表示指令參數 
分為雙後綴（例如movrr,movrm）和單後綴指令（例如addr,addi）
全局 64 位尋址模式，有 ``r0-r254`` 的8字节寄存器和 heap

## 貢獻

## 測試人員
```text
測試人員: AD
測試平台: iOS
環境: 無根越獄環境,基礎開發環境
測試結果: 已和 LMVM 維護者共同確認 lmvm 和 lmlc 在 iOS 上沒有問題, 測試文件全部通過
```

## 許可證

## 注意事項
1. vm/main.cpp，如果因為平台問題無法編譯，嘗試刪除掉計時（chrono）相關代碼(本復刻倉庫已默認注釋掉)

---

## 特性
- ✅ **高性能**：基於 C++ 高效記憶體操作
- ✅ **可擴展**：模組化指令集架構

---