在 ROC_RK3568_PC 上测试 ZVM 性能
==================================

ZVM 的性能测试在瑞芯微 RK3568 芯片上进行（开发板采用 ROC-RK3568-PC），RK3568 芯片是一款基于 ARMv8.2 架构（采用 Cortex A55）的四核 CPU 国产 SoC 芯片。
本次性能测试主要对比虚拟机操作系统（VM）在 **裸板**（即 RK3568 裸板）和 **ZVM**（即 ZVM on RK3568）、Xvisor（Xvisor on RK3568）以及 KVM（Linux-KVM on RK3568）上虚拟机的各种性能表现，
包括系统线程的调度时延、中断处理时延、内核对象的操作时间、系统吞吐量等。
本次测试采用的 VM 为 Zephyr RTOS（3.7 LTS）和 Debian（Linux 5.10 内核），使用的测试集包括：

1. Zephyr Latency Measure 测试集
2. Debian Cyclictest 测试集
3. Zephyr Sys Kernel 测试集
4. Debian Unixbench 测试集

前两个测试集关注 VM 的系统延迟，后两个测试集关注 VM 的整体性能，具体说明见下文。

测试 1：Zephyr RTOS 的延迟测试
---------------------------------

我们分别在裸板、ZVM、Xvisor 和 Linux-KVM 上运行 Zephyr RTOS，并在 Zephyr RTOS 中运行 Zephyr Latency Measure 测试集。
该基准测试测量选定内核功能的平均延迟，如线程上下文切换、中断处理、信号量收发的延迟。
测试项的细节见 `Zephyr Latency Measure <https://github.com/zephyrproject-rtos/zephyr/tree/main/tests/benchmarks/latency_measure>`_ 。

.. image:: https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Zephyr_latency_measure.png
   :align: center
   :width: 700
   :height: 380

**Zephyr 在不同平台上运行 Latency Measurement 测试集的测试结果**

测试结果如上图所示，横坐标代表各个基准项目的时延（单位 ns），纵坐标为各个基准测试项。结果显示，相比于裸机，Zephyr RTOS 运行在 ZVM 上时虚拟化开销极小，而 Xvisor 的平均延迟开销高出 3.78 倍。
尽管 Linux-KVM 在某些情况下与 ZVM 相当，但在中断密集型场景中，它的延迟更高（最大达到了 ZVM 的 4.3 倍），这凸显了 ZVM 在中断处理及延迟敏感操作方面的优化优势。

测试 2：Debian 的延迟测试
------------------------------

我们分别在裸板、ZVM、Xvisor 和 Linux-KVM 上运行 Debian，并在 Debian 中运行 Cyclictest 测试集。
Cyclictest 是一个用于测量 Debian 系统实时性能的工具，尤其是测量系统的 **中断延迟** 和 **调度延迟**，见 `Cyclictest <https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cyclictest/test-design>`_ 详细描述。

在测试过程中，我们还通过 stress 工具逐步增加 Debian 的负载，依次将 CPU 密集型线程数量从 1 个增加到 8 个，每次 Cyclictest 测试的运行时间为 30 分钟，累计运行时间 4 小时。
我们同时使用 stress 生成固定数量的 IO 和内存密集型任务，以模拟真实部署场景。

.. image:: https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Debian_cyclictest.png
   :align: center
   :width: 500
   :height: 600

**Debian 在不同平台上运行 Cyclictest 测试集的测试结果**

测试结果如上图所示，横坐标为 CPU 密集型线程数量，纵坐标为具体的延迟数据指标（单位 us）。
结果显示，与裸机执行相比，ZVM 的最大抖动增加了 6.5%，平均延迟增加了 4.6%。
相比之下，Xvisor 的最大抖动增加了 11%，平均抖动增加了 9.3%，而 Linux-KVM 的最大抖动增加了 157.2%，平均抖动增加了 152.6%。
与 Xvisor 和 Linux-KVM 相比，ZVM 对最大延迟和抖动的控制更为出色，突出了其对最坏情况执行时间的优化处理以及对延迟关键环境的适用性。

测试 3：Zephyr RTOS 开销测试
----------------------------------

我们分别在裸板和 ZVM 上运行 Zephyr RTOS，并在 Zephyr RTOS 中运行 Zephyr sys kernel 基准测试集。
该测试集主要评估 Sem、LIFO、FIFO、堆栈和 Memslab 等内核对象的操作效率，以体现系统开销，见 `Zephyr sys kernel <https://github.com/zephyrproject-rtos/zephyr/tree/main/tests/benchmarks/sys_kernel>`_ 详细描述。

.. image:: https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Zephyr_sys_kernel.png
   :align: center
   :width: 700
   :height: 200

**Zephyr 在不同平台上运行 Sys kernel 测试集的测试结果**

测试结果显示，相比于裸机平台，ZVM 平均开销仅增加 1.8%。相比之下，Xvisor 和 KVM 的平均开销明显更高，分别为 178.4% 和 352.4%。
这些结果表明，ZVM 具有高效的虚拟化处理能力，在性能开销方面明显优于 Xvisor 和 KVM。

测试 4：Debian 开销测试
---------------------------

我们分别在裸板、ZVM、Xvisor 和 KVM 上运行 Debian，并在 Debian 中运行 UnixBench 基准测试集。
UnixBench 套件包含多项测试，每项测试评估 Debian 性能的不同方面，并提供相应的评分。
最终，这些评分会被加权并平均，生成一个综合分数作为系统整体性能的评估值，即 **系统基准指数分数（System Benchmarks Index Score）**，详细描述见 `UnixBench <https://github.com/kdlucas/byte-unixbench>`_ 。

.. image:: https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Debian_unixbench.png
   :align: center
   :width: 700
   :height: 280

**Debian 中运行 UnixBench 测试结果**

测试结果显示，在 **System Benchmarks Index Score** 这个整体分数上，裸机平台得分最高，为 395.8，ZVM 紧随其后，得分为 393.1，开销仅为 0.68%。
相比之下，Xvisor 得分明显较低，为 364.9，开销为 7.8%，而 KVM 得分为 383.3，开销适中，为 3.2%。

`Prev>> 在 ARM FVP 上运行 ZVM <https://gitee.com/openeuler/zvm/blob/master/zvm_doc/5_Run_on_ARM64_FVP.rst>`_
