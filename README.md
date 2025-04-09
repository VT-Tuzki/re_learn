# Re_Learn

> 捡起枝枝蔓蔓 - Picking up the scattered branches and vines

*Last Updated: 2024/12/31*

## 项目愿景

构建一个完整的C语言学习与开发环境，具备：
- 完善的测试框架
- Linux程序编译与验证
- 多处理器支持
- 从简单到复杂的编译流程演进（gcc → make → cmake）

编译目标：
- 基础：gcc直接编译
- 中级：使用make构建系统，集成CMockery测试框架
- 高级：转向cmake构建系统，支持复杂项目结构

测试目标：
- 集成CMockery测试框架
- 自动生成Linux C测试验证程序
- 完整的单元测试与集成测试覆盖


## 项目结构

```
re_learn/
├── apps/                # 应用程序目录，每个应用有自己的子目录
│   ├── VEMU/           # RISC-V RV32I指令集模拟器
│   ├── lib_vtuzki_test/ # VtuzkiCC库测试应用
│   └── template_app/   # 应用程序模板
├── cmake/               # CMake工具函数和模块
├── common/              # 公共代码库
├── external/            # 外部依赖项
│   ├── vtuzkicc/       # C通用数据结构容器库
│   └── cmockery/       # 单元测试框架
├── include/             # 全局头文件
├── src/                 # 源代码
├── tests/               # 项目级测试
├── build/               # 构建输出目录（自动生成）
└── CMakeLists.txt       # 主CMake配置文件
```

## 当前应用状态
### VEMU (RISC-V RV32I 模拟器)
- 基础架构基本完成
- 内存\ 寄存器系统已经实现
- 指令取指 译码 运行相关模块已完成
- 执行单元正在开发中 目前只有 addi add sub命令
- 待完成 完整RV32I 指令集 调试工具 测试工具

### lib_vtuzki_test
- 是另一个仓库libvtuzkicc 作为子模块的一个测试项目

### template_app
- 是新建项目时的模板应用

## 使用方法

### 环境准备
```bash
# 安装必要的依赖
sudo apt-get install build-essential cmake libcmockery-dev
```

### 使用CMake构建项目
```bash
# 创建并进入构建目录
mkdir -p build && cd build

# 配置项目
cmake ..

# 构建项目
cmake --build .

# 运行所有测试
ctest

# 只运行特定应用的测试
ctest -R <app_name>
```

### CMake构建选项
```bash
# 禁用应用构建
cmake -DRE_LEARN_BUILD_APPS=OFF ..

# 禁用测试构建
cmake -DRE_LEARN_BUILD_TESTS=OFF ..

# 设置构建类型
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake -DCMAKE_BUILD_TYPE=release ..
cmake -B build -S . -DCMAKE_BUILD_TYPE=MinSizeRel
# 只构建特定的应用
cmake -DRE_LEARN_SELECTED_APPS=lib_vtuzki_test ..

# 构建多个特定应用（用分号或逗号分隔）
cmake -DRE_LEARN_SELECTED_APPS="lib_vtuzki_test,other_app" ..
```

## 项目来源

本项目整合了多个学习资源与实践经验：

1. 初期复刻[一点五编程的通用C容器项目](https://github.com/wallacegibbon/container-collection-for-c.git)，当前的C通用数据结构实现脱胎于此
2. 借鉴《Learn C the Hard Way》项目，改进了Makefile和shell脚本，增加了静态/动态库输出
3. 学习[一生一芯(ysyx)项目](http://ysyx.org/)的makefile目录结构，研究了automake流程，重构了makefile的include树结构，增加了自动配置项
4. 升级到现代CMake构建系统，实现模块化组织和自动化测试
5. 实现RV32I模拟器 , 学习计算机体系结构和处理器设计
## 未来计划

1. 测试流程改进
   - 利用CTest进一步整合自动化测试
   - 实现GitHub Actions CI/CD流程，自动运行CMake构建和测试
   - 集成CMake与gcov，增加代码覆盖率报告

2. CMake构建系统增强
   - 添加CPack支持，实现打包和发布
   - 实现跨平台构建配置
   - 支持更多外部依赖的自动检测与集成
   - 添加编译选项预设(presets)配置

3. 开发环境改进
   - 增加CMake配置检查功能，验证开发环境完整性
   - 添加VSCode CMake Tools集成配置
   - 实现compile_commands.json生成，支持现代IDE和编辑器

4. 文档完善
   - 使用Doxygen生成API文档
   - 为每个模块添加CMake集成示例
   - 添加性能基准测试框架





