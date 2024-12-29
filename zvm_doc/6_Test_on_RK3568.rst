在ROC_RK3568_PC上测试ZVM性能
======================

#### 测试1：**Zephyr RTOS的延迟测试**

我们分别在裸板和ZVM上运行Zephry RTOS，并在ZephyrRTOS中运行Zephyr Latency measure测试集，该基准测试测量选定内核功能的平均延迟，如线程上下文切换、中断处理、信号量收发的延迟，测试项的细节见：[Zephyr Latency measure](https://github.com/zephyrproject-rtos/zephyr/tree/main/tests/benchmarks/latency\_measure)中列出的基准项。

<p align="center"><img width="700" height="380"  src="https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Zephyr_latency_measure.png"/>
<p align="center">   Zephyr中运行Latency Measure测试结果

测试结果如上图所示，横坐标代表各个基准项目的时延（单位为ns），纵坐标为各个基准测试项。结果显示，相比于裸机，Zephyr RTOS运行在ZVM上时，“Context switch between preemptive thread”测试项的延迟增加了7.1%， “Abort thread”测试项增加了2.4%，所有其他测试项延迟增幅均低于1%,且大部分是0增长。整体结果表明，ZVM对Zephyr RTOS的各项基准测试延迟的影响很小。

#### 测试2：**Debian的延迟测试**

我们分别在裸板和ZVM上运行Debian，并在Debian中运行Cyclictest测试集，Cyclictest 是一个用于测量 Debian系统实时性能的工具，尤其是测量系统的**中断延迟**和**调度延迟**。它通常用于测试系统在实时场景下的响应能力，以确保系统在关键任务中具备足够的实时性。见[Cyclictest](https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cyclictest/test-design)详细描述。在测试过程中，我们还通过stress工具逐步增加Debian的负载，依次将CPU密集型线程数量从1个增加到8个，每次Cyclictest测试的运行时间为30分钟，累计运行时间4小时。我们同时使用stress生成固定数量的IO和内存密集型任务，以模拟真实部署场景。

<p align="center"><img width="500" height="300"  src="https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Debian_cyclictest.png"/>
<p align="center">   Debian中运行Cyclictest测试结果

测试结果如上图所示，横坐标为CPU密集型线程数量，纵坐标为具体的延迟数据指标（单位为us）。我们选择1，2，4，8等四个CPU密集型任务数量作为采样点。结果显示，相比于裸机，Debian运行在ZVM上时，最大延迟增加了6.2%，最小延迟增加了2.4%。随着系统负载的增加，Debian在ZVM运行的Cyclictest测试结果逐渐逼近裸机。具体原因是负载较轻情况下ZVM本身内核的线程存在资源竞争会会对Debian线程有一定影响，而当负载到达一定临界值时，裸机与ZVM的资源竞争都满量。

#### 测试3：**Zephyr RTOS开销测试**

我们分别在裸板和ZVM上运行Zephyr RTOS，并在Zephyr RTOS中运行Zephyr sys kernel基准测试集，其主要评估Sem、LIFO、FIFO、堆栈和Memslab等内核对象的操作效率，以体现系统开销。见[Zephyr sys kernel](https://github.com/zephyrproject-rtos/zephyr/tree/main/tests/benchmarks/sys_kernel)详细描述。

<p align="center"><img width="580" height="360"  src="https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Zephyr_sys_kernel.png"/>
<p align="center">   Zephyr中运行Sys kernel测试结果

测试结果如上图所示，横坐标为各项基准测试项目，纵坐标为相关执行项的时间（单位为ns）。测试结果显示，相比于裸机平台，ZVM上运行Zephy RTOS的Semaphore 3测试项的损耗最大（8.7%），LIFO 3项的性能损耗为5.4%，其余损耗均在3%以内，且FIFO/Stack/Memslab等多个测试项的损耗为0。相比于裸机，ZVM引入的平均开销损耗仅为1.4%，说明ZVM上运行Zephyr RTOS的性能达到了裸机平台的98.6%。

#### 测试4：**Debian开销测试**

我们分别在裸板和ZVM上运行Debian，并在Debian中运行UnixBench基准测试集。UnixBench套件包含多项测试，每项测试评估Debian性能的不同方面，并提供相应的评分。最终，这些评分会被加权并平均，生成一个综合分数作为系统整体性能的评估值，即系统基准指数分数（System Benchmarks Index Score），详细描述见[UnixBench](https://github.com/kdlucas/byte-unixbench)。

<p align="center"><img width="700" height="320"  src="https://gitee.com/openeuler/zvm/raw/master/zvm_doc/figure/Debian_unixbench.png"/>
<p align="center">   Debian中运行Unixbench测试结果

测试结果如上图所示，横坐标为各项基准测试项，纵坐标各个基准项的评分（Score数值）。测试结果显示，相比于裸机平台，ZVM上运行Debian的Process Creation测试项所引入的性能损耗最大（5.7%），其余项的性能损耗都在2%以内。ZVM和裸机对比，基准指数分数上的开销损耗仅为0.7%。说明ZVM上运行Debian的性能达到了裸机平台的99.3%，基本可以忽略不计。

`Prev>> 在ARM FVP上运行ZVM <https://gitee.com/openeuler/zvm/blob/master/zvm_doc/6_Test_on_RK3568.rst>`__