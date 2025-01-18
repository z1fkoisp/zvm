在ROC_RK3568_PC上测试ZVM性能
======================

ZVM的性能测试在瑞芯微RK3568芯片上进行（开发板采用ROC-RK3568-PC），RK3568芯片是一款代表性的基于ARMv8.2架构（采用Cortex A55）的4核CPU国产SoC芯片。
本次性能测试主要对比虚拟机操作系统（VM）在**裸板**（即RK3568裸板）和**ZVM**（即ZVM on RK3568）、
Xvisor（Xvisor on RK3568）以及KVM（Linux-KVM on RK3568）上虚拟机的各种性能表现，
包括系统线程的调度时延、中断处理时延、内核对象的操作时间、系统吞吐量等。
本次测试采用的VM为Zephyr RTOS（3.7 LTS）和Debian（Linux 5.10内核），使用的测试集包括：1）Zephyr Latency  Measure测试集；2）Debian Cyclictest测试集；3）Zephyr Sys kernel测试集；4）Debian Unixbench测试集。前两个测试集关注VM的系统延迟，后两个测试集关注VM的整体性能，具体说明见下面描述。

#### 测试1：**Zephyr RTOS的延迟测试**

我们分别在裸板、ZVM、Xvisor和Linux-KVM上运行Zephry RTOS，并在Zephyr RTOS中运行Zephyr Latency measure测试集，该基准测试测量选定内核功能的平均延迟，如线程上下文切换、中断处理、信号量收发的延迟，测试项的细节见：[Zephyr Latency measure](https://github.com/zephyrproject-rtos/zephyr/tree/main/tests/benchmarks/latency\_measure)中列出的基准项。

<p align="center"><img width="700" height="380"  src="https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Zephyr_latency_measure.png"/>
<p align="center">   Zephyr在不同平台上运行Latency Measurement 测试集的测试结果

测试结果如上图所示，横坐标代表各个基准项目的时延（单位为ns），纵坐标为各个基准测试项。结果显示，相比于裸机，Zephyr RTOS运行在ZVM上时虚拟化开销极小，而Xvisor的平均延迟开销高出3.78倍。 尽管Linux-KVM在某些情况下与 ZVM 相当，但在中断密集型场景中，它的延迟更高（最大达到了ZVM的4.3倍），这凸显了 ZVM 优化的中断处理及其在延迟敏感操作中的卓越性能。

#### 测试2：**Debian的延迟测试**

我们分别在裸板、ZVM、Xvisor和Linux-KVM上运行Debian，并在Debian中运行Cyclictest测试集，Cyclictest 是一个用于测量 Debian系统实时性能的工具，尤其是测量系统的**中断延迟**和**调度延迟**。它通常用于测试系统在实时场景下的响应能力，以确保系统在关键任务中具备足够的实时性。见[Cyclictest](https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cyclictest/test-design)详细描述。在测试过程中，我们还通过stress工具逐步增加Debian的负载，依次将CPU密集型线程数量从1个增加到8个，每次Cyclictest测试的运行时间为30分钟，累计运行时间4小时。我们同时使用stress生成固定数量的IO和内存密集型任务，以模拟真实部署场景。

<p align="center"><img width="500" height="600"  src="https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Debian_cyclictest.png"/>
<p align="center">   Debian在不同平台上运行Cyclictest测试集的测试结果

测试结果如上图所示，横坐标为CPU密集型线程数量，纵坐标为具体的延迟数据指标（单位为us）。我们选择1，2，4，8等四个CPU密集型任务数量作为采样点。结果显示，与裸机执行相比，ZVM 的最大抖动增加了 6.5%，平均延迟增加了 4.6%。相比之下，Xvisor 的最大抖动增加了 11%，平均抖动增加了 9.3%，而 Linux-KVM 的最大抖动增加了 157.2%，平均抖动增加了 152.6%。与 Xvisor 和 Linux-KVM 相比，ZVM 对最大延迟和抖动的控制更为出色，突出了其对最坏情况执行时间的优化处理以及对延迟关键环境的适用性。

#### 测试3：**Zephyr RTOS开销测试**

我们分别在裸板和ZVM上运行Zephyr RTOS，并在Zephyr RTOS中运行Zephyr sys kernel基准测试集，其主要评估Sem、LIFO、FIFO、堆栈和Memslab等内核对象的操作效率，以体现系统开销。见[Zephyr sys kernel](https://github.com/zephyrproject-rtos/zephyr/tree/main/tests/benchmarks/sys_kernel)详细描述。

<p align="center"><img width="700" height="200"  src="https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Zephyr_sys_kernel.png"/>
<p align="center">   Zephyr在不同平台上运行Sys kernel测试集的测试结果

测试结果如上图所示，横坐标为各项基准测试项目，纵坐标为相关执行项的时间（单位为ns）。测试结果显示，相比于裸机平台，ZVM平均开销仅增加 1.8%。相比之下，Xvisor 和 KVM 的平均开销明显更高，分别为 178.4% 和 352.4%。这些结果说明了 ZVM 高效的虚拟化处理能力以及与 Xvisor 和 KVM 相比来看有更好的性能。

#### 测试4：**Debian开销测试**

我们分别在裸板，ZVM, Xvisor和KVM上分别运行Debian，并在Debian中运行UnixBench基准测试集。UnixBench套件包含多项测试，每项测试评估Debian性能的不同方面，并提供相应的评分。最终，这些评分会被加权并平均，生成一个综合分数作为系统整体性能的评估值，即系统基准指数分数（System Benchmarks Index Score），详细描述见[UnixBench](https://github.com/kdlucas/byte-unixbench)。

<p align="center"><img width="700" height="280"  src="https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Debian_unixbench.png"/>
<p align="center">   Debian中运行Unixbench测试结果

测试结果如上图所示，横坐标为各项基准测试项，纵坐标各个基准项的评分（Score数值）。测试结果显示，在”System Benchmarks Index Score“这个整体分数上，裸机平台得分最高，为 395.8，ZVM 紧随其后，得分为 393.1，开销仅为 0.68%。相比之下，Xvisor 得分明显较低，为 364.9，开销为 7.8%，而 KVM 得分为 383.3，开销适中，为 3.2%。

`Prev>> 在ARM FVP上运行ZVM <https://gitee.com/openeuler/zvm/blob/master/zvm_doc/6_Test_on_RK3568.rst>`__