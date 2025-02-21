# re_learn
捡起枝枝蔓蔓
2024/12/31

憧憬： 拼接成一个有测试框架，有linux程序编译验证，支持多处理器的一个编译环境
编译流程 gcc 过渡make 过渡cmake
测试 过渡make的时候把 CMockery加入  中途尝试增加 对特殊需求函数体 同步生成linuxC程序验证
支持多处理器， 在一生一芯学习到os的时候进行加入

这个憧憬就不更新了 后面每次看到都能反省下


使用方法
待整理

本文件源自多个项目整合
1. 初期复刻一点五编程的通用C容器项目 https://github.com/wallacegibbon/container-collection-for-c.git 目前的C通用数据结构也是脱胎于此项目
2. 迭代时重新复习C语言, 故在 `Learn_C_the_hard_way` 项目中汲取了灵感, 改装了Makefile以及sh脚本,同时增加静态\动态库输出
3. 学习ysyx项目的时候研究了该项目的makefile目录结构,同时去研究了automake的流程, 变更了makefile的include树结构.增加了一些自动配置项
4. 待定
   1. 更新自动化测试流程, 学习github action流程
   2. 更新make构建流程, 重新翻别的大型多仓库项目代码, 看看怎么能让这个项目直接扔进去改改上层构建参数就能运行
   3. 增加`make check/configre.sh check`等流程, 环境检测
   4. 待定





