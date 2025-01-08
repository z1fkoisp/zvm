# ZVM 开源文档

[![License](https://img.shields.io/badge/license-Apache%202-blue.svg)](LICENSE)

ZVM（Zephyr-based Virtual Machine）是新一代Type 1.5 嵌入式RTOS虚拟化解决方案，它结合开源微内核实时操作系统 [Zephyr RTOS](https://github.com/zephyrproject-rtos/zephyr) 开发，能在单一硬件平台上启动多个操作系统，以构建安全隔离的多内核混合部署系统，为多OS和多任务处理提供实时与灵活的虚拟化支持。Type 1.5并不是实时性（Type 1）与灵活性（Type 2）之间的权衡，而是在实时性与灵活性方面都达到最佳状态，且不牺牲任何一方。

（1）ZVM并非附加于Zephyr RTOS之上，而是直接运行在硬件之上，不仅共享Zephyr RTOS内核的开源生态、驱动支持和调度能力（相比Type 1具有更高的灵活性），而且避免了Type 2因多层依赖导致的时延开销（相比Type 2具有更强的实时性）。

（2）通过Zephyr RTOS的硬实时调度机制与ZVM的任务隔离机制相结合，确保实时任务不受低优先级任务干扰（相比Type 1及Type 2均具有更强的实时性）。

ZVM支持同时运行多个同类型或不同类型的Guest OS，且支持多核Guest OS。
ZVM项目仓库中包含Zephyr RTOS内核、工具及虚拟化支持代码。

## 架构设计

ZVM面向高性能嵌入式计算环境，提供嵌入式平台上操作系统级别的资源隔离和共享服务。可用于各种应用和行业领域，如工业控制、机器人、汽车电子、轨道交通等。ZVM架构图如下所示：

<p align="center"><img width="600" height="500"  src="https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/overview.png"/>

## 持续集成

ZVM将持续支持多种虚拟机操作系统和底层硬件平台，拓展软硬件生态。

#### 虚拟机操作系统（Guest OS）支持

ZVM目前支持运行多款Guest OS，包括：

- Zephyr RTOS、FreeRTOS
- openEuler Embedded、Debian GNU/Linux

#### 底层芯片支持

ZVM目前支持兼容ARMv8架构的处理器芯片，包括：

- 瑞芯微RK3568/RK3588
- 飞腾D3000/S5000C
- QEMU ARM64 virt (qemu-max)
- ARM FVP(Fixed Virtual Platform, A55)

## 视频介绍

视频部分主要为ZVM在各类技术分享会议上的录屏，主要介绍了ZVM的架构和功能，方便用户快速了解ZVM。

#### EOSS分享视频

[ZVM: An Embedded Real-time Virtual Machine Based on Zephyr RTOS](https://mp.weixin.qq.com/s/igDKghI7CptV01wu9JrwRA)

#### Sig-Zephyr分享视频

[ZVM: 基于Zephyr RTOS的嵌入式实时虚拟机](https://www.bilibili.com/video/BV1pe4y1A7o4/?spm_id_from=333.788.recommend_more_video.14&vd_source=64410f78d160e2b1870852fdc8e2e43a)

## 快速上手ZVM

#### 1. 系统简介 ：

系统介绍部分主要包含ZVM的总体涉及介绍以及基础的模块支持介绍，方便用户理解ZVM的设计思想和理念，详细内容见如下链接：[System_Design：https://gitee.com/openeuler/zvm/blob/master/zvm_doc/1_System_Design.rst](https://gitee.com/openeuler/zvm/blob/master/zvm_doc/1_System_Design.rst)

#### 2. 开发环境搭建：

环境搭建部分主要介绍Linux/Ubuntu主机开发环境的配置，包括主机各类环境变量的配置与升级、zephyrproject SDK的配置、环境验证、zvm仓库的初始化以及简单zvm sample的构建与测试等，方便用户快速入手搭建开发环境，详细内容见如下链接：[Environment_Configuration：https://gitee.com/openeuler/zvm/blob/master/zvm_doc/2_Environment_Configuration.rst](https://gitee.com/openeuler/zvm/blob/master/zvm_doc/2_Environment_Configuration.rst)

#### 3. 运行ZVM：

我们提供了ZVM在QEMU和RK353568板卡进行验证的指南，可以在上面运行多个虚拟机并进行虚拟化功能测试，下面是ZVM在各个平台上运行的详细教程：

- [ZVM on QEMU：https://gitee.com/openeuler/zvm/blob/master/zvm_doc/3_Run_on_ARM64_QEMU.rst](https://gitee.com/openeuler/zvm/blob/master/zvm_doc/3_Run_on_ARM64_QEMU.rst)
- [ZVM on RK3568：https://gitee.com/openeuler/zvm/blob/master/zvm_doc/4_Run_on_ROC_RK3568_PC.rst](https://gitee.com/openeuler/zvm/blob/master/zvm_doc/4_Run_on_ROC_RK3568_PC.rst)
- [ZVM on FVP：https://gitee.com/openeuler/zvm/blob/master/zvm_doc/5_Run_on_ARM64_FVP.rst](https://gitee.com/openeuler/zvm/blob/master/zvm_doc/5_Run_on_ARM64_FVP.rst)

## ZVM性能测试结果

ZVM的性能测试在瑞芯微RK3568芯片上进行（开发板采用ROC-RK3568-PC），RK3568芯片是一款代表性的基于ARMv8.2架构（采用Cortex A55）的4核CPU国产SoC芯片。本次性能测试主要对比虚拟机操作系统（VM）在**裸板**（即RK3568裸板）和**ZVM**（即ZVM on RK3568）上的各种性能指标，包括系统线程的调度时延、中断处理时延、内核对象的操作时间、系统吞吐量等。本次测试采用的VM为Zephyr RTOS（3.7 LTS）和Debian（Linux 5.10内核），使用的测试集包括：1）Zephyr Latency  Measure测试集；2）Debian Cyclictest测试集；3）Zephyr Sys kernel测试集；4）Debian Unixbench测试集。前两个测试集关注VM的系统延迟，后两个测试集关注VM的整体性能，具体说明见下面测试链接中描述。

- [Test ZVM on RK3568：https://gitee.com/openeuler/zvm/blob/master/zvm_doc/6_Test_on_RK3568.rst](https://gitee.com/openeuler/zvm/blob/master/zvm_doc/6_Test_on_RK3568.md)


## 交流与反馈

扫码加入ZVM技术交流群：

<p align="center"><img width="600" height="600"  src="https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/weixinCode.jpg"/>
<p align="center">   ZVM技术交流微信群


#### 研发团队：

**谢国琪（ZVM项目创始人）**，邮箱：xgqman@hnu.edu.cn, [个人主页](http://csee.hnu.edu.cn/people/xieguoqi)

**熊程来（openEuler SIG-Zephyr maintainer）**，邮箱：xiongcl@hnu.edu.cn

**胡星宇**，邮箱：huxingyu@hnu.edu.cn

**王中甲**，邮箱：zjwang@hnu.edu.cn

**赵思蓉**，邮箱：zhaosr@hnu.edu.cn

**胡宇昊**，邮箱：ahui@hun.edu.cn

**王清桥**，邮箱：qingqiaowang@hnu.edu.cn

**李宗军**，邮箱：lizongjun@phytium.com.cn

**黄鹤**，邮箱：huanghe@phytium.com.cn

**任慰（openEuler SIG-Zephyr maintainer）**，邮箱：dfrd-renw@dfmc.com.cn

欢迎大家反馈开发中遇到的问题，可以联系上面邮箱或者加入技术交流群。

## 版权与许可证

ZVM使用 [zephyrproject-rtos](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fzephyrproject-rtos%2Fzephyr) 所遵守的 [Apache 2.0 许可证](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fzephyrproject-rtos%2Fzephyr%2Fblob%2Fmain%2FLICENSE) ，主要开发语言为C/C++语言。Apache 2.0许可证是一种自由软件许可证，允许用户自由使用、修改和分发软件， 不影响用户的商业使用。

## 参与贡献

ZVM作为Zephyr实时操作系统生态在国内的关键一环，致力于构建国内开源hypervisor生态，且正处于快速发展的时期，我们欢迎对ZVM及Zephyr感兴趣的小伙伴加入本项目。

1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request
