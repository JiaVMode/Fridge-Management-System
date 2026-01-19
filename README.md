# 🧊 Smart Fridge Management System (智能冰箱管理系统)

![C Language](https://img.shields.io/badge/Language-C99-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

## 📖 项目简介 (Introduction)

**智能冰箱管理系统** 是一个基于 C 语言开发的高性能控制台应用程序。它旨在模拟和解决家庭场景下冰箱管理的痛点，如食物过期、空间规划混乱、温度冲突等问题。

本项目不仅仅是一个简单的课程设计，更是一个**工程级的 C 语言实践范本**。它展示了如何仅使用标准库实现：
- 🛡️ **鲁棒的内存管理**：自动化的链表操作，杜绝内存泄漏。
- ⚡ **高效的数据结构**：基于链表的动态库存管理。
- 🎨 **现代化的 TUI**：在控制台中实现像素级对齐的无边框界面。
- 💾 **健壮的数据持久化**：标准 CSV 格式存储，数据永不丢失。

---

## ✨ 核心功能 (Key Features)

### 1. 🧠 智能逻辑核心
- **物理约束模拟**：系统会实时监控冰箱的剩余容积和温度限制。试图放入体积过大或温度不兼容的食物会被自动拦截。
- **智能合并算法**：当存入同名食物时，系统会自动识别并合并体积，同时智能处理温度和种类冲突，无需人工干预。

### 2. 🛡️ 极致防御性编程
- **输入防爆**：重写了底层输入函数 (`get_valid_*`)，无论是乱码、超长字符串还是非法字符，都无法让系统崩溃。
- **内存安全**：所有字符串操作均采用安全函数（如 `strncpy`），杜绝缓冲区溢出。

### 3. 🎨 优雅的用户体验
- **自动清屏**：智能状态机管理，每次操作后自动刷新界面，保持视野清爽。
- **单键交互**：优化了 I/O 缓冲区管理，实现“回车即响应”，告别繁琐的二次确认。
- **经典青色主题**：采用了精心调配的 `Consolas` 青色配色，专业且护眼。

---

## 🛠️ 技术架构 (Technical Architecture)

本项目采用模块化分层架构设计：

```mermaid
graph TD
    User[用户输入] --> UI[UI 交互层 (ui.c)]
    UI --> Controller[业务调度层 (main.c)]
    Controller --> Logic[核心逻辑层 (fridge.c)]
    Logic --> Data[数据结构层 (food.c)]
    Logic --> Storage[持久化层 (file_io.c)]
    Storage <--> CSV[foods.csv 文件]
```

- **src/main.c**: 程序入口与主循环调度。
- **src/ui.c**: 负责 ASCII 界面渲染与鲁棒输入处理。
- **src/fridge.c**: 核心业务逻辑（增删改查、排序、统计）。
- **src/food.c**: 内存节点的生命周期管理。
- **src/file_io.c**: CSV 文件的序列化与反序列化。

---

## 🚀 快速开始 (Quick Start)

### 环境要求
- Windows 操作系统
- GCC 编译器 (MinGW)

### 编译指南
在项目根目录下打开终端，运行以下命令：

```bash
gcc src/main.c src/food.c src/fridge.c src/file_io.c src/ui.c -o fridge_system.exe
```

### 运行
```bash
./fridge_system.exe
```

---

## 📂 项目结构 (Project Structure)

```text
Fridge-Management-System/
├── data/
│   └── foods.csv          # 📦 数据库文件 (Excel可读)
├── include/
│   ├── food.h             # 🥬 食物节点定义
│   ├── fridge.h           # ❄️ 冰箱操作声明
│   ├── file_io.h          # 💾 文件读写接口
│   └── ui.h               # 🖥️ UI工具声明
├── src/                   # 📝 源代码目录
├── report.md              # 📑 深度技术报告 (Dissertation)
└── README.md              # 📘 项目说明文档
```


## 📄 许可证 (License)

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。
