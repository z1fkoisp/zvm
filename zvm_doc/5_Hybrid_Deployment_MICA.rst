基于 ZVM 混合部署框架
======================
当前 openEuler Embedded 支持 ZVM 上的混合部署，可以通过 openEuler Embedded 对 Zephyr RTOS 生命周期进行管理，并实现虚拟机间通过 IVSHMEM 通信。

1.  构建说明
-----------------------

目前仅支持`arm64(aarch64) qemu`，参考[openEuler Embedded MCS镜像构建指导](https://openeuler.gitee.io/yocto-meta-openeuler/master/features/mica/build.html#mcs-build)。

对于ZVM，oebuild 的编译配置文件`compile.yaml` 中 MCS_FEATURES 为 openamp。

1）解压构建得到的根文件系统：

.. code:: shell

    gzip -d openeuler-image-qemu-aarch64-*.rootfs.cpio.gz
    cpio -idmv < openeuler-image-qemu-aarch64-*.rootfs.cpio


2）将 ZVM 仓库中 zvm.ko 内核模块复制到根文件系统 `/lib/modules/5.10.0/drive` 目录中，混合部署测试程序 zvm_mica 复制到 `/usr/bin` 目录，并执行 depmod 生成模块映射文件以更新模块依赖：

.. code:: shell

    cp <zvm_workspace>/zvm_config/qemu_platform/hub/zvm.ko <oee_rootfs>/lib/modules/5.10.0/drive
    cp <zvm_workspace>/zvm_config/qemu_platform/hub/zvm_mica <oee_rootfs>/usr/bin

    depmod -a -b /path/to/unpacked/rootfs/ <kernel_version>

3）重新打包根文件系统并将其复制到镜像内核源码中：

.. code:: shell

    sudo find . -print0 | sudo cpio --null -ov --format=newc | sudo gzip -9 > ../initramfs.cpio.gz

    cp initramfs.cpio.gz <build_mcs>/tmp/work-shared/qemu-aarch64/kernel-source 

4）拷贝原 MCS 镜像内核配置文件，在内核选项中指定根文件系统路径将其编译入内核并关闭 Kernel support for 32-bit EL0 选项：

.. code:: shell

    cp <build_mcs>/tmp/work/qemu_aarch64-openeuler-linux/linux-openeuler/5.10-r0/build <build_mcs>/tmp/work-shared/qemu-aarch64/kernel-source
    # 内核选项调整
    # - "General setup"：
    #    - "Initial RAM filesystem and RAM disk (initramfs/initrd) support"：选择 "Y" 以启用。
    #   - "Initramfs source file(s)"：指定根文件系统的路径 `./initramfs.cpio.gz`
    # - "Kernel Features"
    #    - "Kernel support for 32-bit EL0"中的所有选项关闭

    # 进入构建容器
    oebuild bitbake

    make ARCH=arm64 CROSS_COMPILE=/usr1/openeuler/gcc/openeuler_gcc_arm64le/bin/aarch64-openeuler-linux-gnu- -j8

5）将编译生成的 openEuler Embedded 镜像拷贝至 ZVM 目录:

.. code:: shell

    cp <build_mcs>/tmp/work-shared/qemu-aarch64/kernel-source/arch/arm64/boot/Image <zvm_workspace>/zvm_config/qemu_platform/hub/Image

2. 如何在 ARM64 QEMU 上运行

1）启动 ZVM：

执行 ZVM 目录下脚本运行 ZVM：

.. code:: shell

    ./auto_zvm.sh debug qemu

2）部署 openEuler Embedded：

通过 ZVM 创建并启动 openEuler Embedded：

.. code:: shell

    zvm_host:~#zvm new -t linux
    Ready to create a new vm..** Create VM instance successful!
    ** Init VM ops successful!
    ** Init VM irq block successful!
    ** Init VM vcpus instances successful!
    ** Add UART_1 device to vm successful.
    Device name: vm_gic_v3.
    ** List register num: 4
    ** Init VM devices successful!
    ** Init VM memory successful!

    |*********************************************|
    |******  Create vm successful!  **************|
    |******          VM INFO                ******|
    |******  VM-NAME:     linux_os-2        ******|
    |******  VM-ID:          2              ******|
    |******  VCPU NUM:       1              ******|
    |******  VMEM SIZE:      1024(M)        ******|
    |*********************************************|

    zvm_host:~#zvm run -n 2
    ** Ready to run VM.

    |*********************************************|
    |******  Start vm successful!  ***************|
    |******          VM INFO                ******|
    |******  VM-NAME:     linux_os-2        ******|
    |******  VM-ID:          2              ******|
    |******  VCPU NUM:       1              ******|
    |*********************************************|

    ** Start running vcpu: linux_os-2-0.

3）部署 Client OS：

openEuler Embedded 载入 ZVM 混合部署内核模块，利用 zvm_mica 程序测试虚拟机生命周期管理功能：

.. code:: shell

    qemu-aarch64 / # insmod zvm.ko

    # 创建Zephyr
    qemu-aarch64 / # ./zvm_mica create
    # 启动Zephyr
    qemu-aarch64 / # ./zvm_mica start

4）打开 Client OS 的 shell：

启动 Zephyr 后，通过 screen 接入 Client OS 的 shell：

.. code:: shell

    screen /dev/pts/9

    ... ...

    uart:~$ kernel uptime
    Uptime: 1548050 ms
    uart:~$ kernel version
    Zephyr version 2.7.99

`Prev>> 在RK3568上运行ZVM <https://gitee.com/openeuler/zvm/blob/master/zvm_doc/4_Run_on_ROC_RK3568_PC.rst>`__

`Next>> 加入我们 <https://gitee.com/openeuler/zvm/blob/master/zvm_doc/6_Join_us.rst>`__