在ARM FVP Model上运行ZVM
======================

1.  构建面向fvp_cortex_a55x4_a75x2平台的ZVM镜像
-----------------------

拉取镜像并进入工作区：

.. code:: shell

   cd ~/zvm_workspace/zvm

1） 使用脚本文件构建ZVM镜像：

.. code:: shell

   ./auto_zvm.sh build fvp_cortex_a55x4_a75x2_smp

或者使用命令行构建镜像:

.. code:: shell

   west build -b fvp_cortex_a55x4_a75x2_smp samples/_zvm


2） 生成ZVM镜像文件如下:

.. code:: shell

    build/zephyr/zvm_host.elf


2. FVP Model 平台下载
-------------------------------

这里使用的平台是 Foundation_Platform, 该平台是ARM提供的一个FVP模型，可以模拟ARM的处理器。
具体可以通过如下方式下载：

1）进入 www.arm.com 的官网，并注册账号（arm 的免费软件需要注册才能下载）。

2）然后访问如下地址下载- `[Armv-A Foundation AEM FVP (x86 Linux)]
<https://developer.arm.com/-/cdn-downloads/permalink/Fixed-Virtual-Platforms/FM-11.27/Foundation_Platform_11.27_19_Linux64.tgz>`__

3） 下载完成后，可以在如下网址找到使用文档：`[Fixed Virtual Platforms] <https://developer.arm.com/Tools%20and%20Software/Fixed%20Virtual%20Platforms>`__ 。
随后，解压到自己的软件下载目录(建议直接放到zvm_workspace/zvm/zvm_config/fvp_platform/hub目录下，后面直接使用脚本即可)：

.. code:: shell

    tar zxvf Foundation_Platform_11.27_19_Linux64_armv8l.tgz

4) 解压后，可以在models目录下找到 `Foundation_Platform` 可执行文件，可以在该目录下运行如下命令测试是否安装成功：

.. code:: shell

   ./Foundation_Platform --image ../../examples/hello.axf

成功会输出`Hello, 64-bit world!`。


3. FVP Model 平台运行ZVM
-------------------------------

首先拉取需要使用的镜像并进入工作区：

.. code:: shell
    cd ../                             #返回到zvm_workspace
    git clone https://gitee.com/hnu-esnl/zvm_vm_image.git

其中，需要的镜像在fvp_platform目录下：

.. code:: shell

    zvm_vm_image/fvp_platform/hub/*  #启动需要的镜像文件

将其复制到zvm_workspace/zvm/zvm_config/fvp_platform/hub目录，然后进入zvm_workspace/zvm目录下，
执行如下命令开始调试：

.. code:: shell

    ./auto_zvm.sh debugserver fvp_cortex_a55x4_a75x2_smp

等待弹出zvm_host#的窗口即可。

`Prev>> 在RK3568上运行ZVM <https://gitee.com/openeuler/zvm/blob/master/zvm_doc/4_Run_on_ROC_RK3568_PC.rst>`__

`Next>> 在RK3568上测试ZVM性能 <https://gitee.com/openeuler/zvm/blob/master/zvm_doc/6_Test_on_RK3568.rst>`__
