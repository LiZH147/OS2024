## 实验一 openEuler 操作系统 编译内核实验

### 实验过程

#### openEuler操作系统安装

1. [下载openEuler 22.03 LTS SP3树莓派版本](https://repo.openeuler.org/)

   **一定要确保版本号一致**

   ![image-20240501215842639](C:\Users\123\AppData\Roaming\Typora\typora-user-images\image-20240501215842639.png)

2. 烧录系统

   使用Raspberry Pi Imager进行系统烧录。将SD卡使用读卡器插入电脑，不需要选择Raspberry Pi Device选项，直接在选择操作系统中选择下载好的系统。

   ![image-20240501220010304](C:\Users\123\AppData\Roaming\Typora\typora-user-images\image-20240501220010304.png)

   在设置环节，无需进行特别设置，全部默认即可，设置了也没用。

   **系统烧录完成后，重新插入SD卡，在根目录下新建ssh文件（没有后缀），以确保能正常进行ssh连接。**

3. 使用网线连接网络

   此步主要是获取树莓派的IP地址，然后使用ssh进行连接。因为树莓派没有可操作的屏幕和键盘，不能进行系统设置。

   将SD卡插入树莓派中，用网线将树莓派与电脑相连，连接树莓派电源。

   + win10系统下

     打开**控制面板**， 选择**网络和Internet**，选择**网络和共享中心**

     ![image-20240501220623082](C:\Users\123\AppData\Roaming\Typora\typora-user-images\image-20240501220623082.png)

     将电脑连接的网络通过网线共享给树莓派，这样树莓派能连接上网络，电脑就可以通过ssh连接树莓派了。图中绿色箭头所指应为**树莓派连接电脑后占用的网络**

     ![网线共享网络](D:\李梓涵\2024春季学期\操作系统原理\实验3\网线共享网络.jpg)

     连接成功如图：

     ![连接成功](D:\李梓涵\2024春季学期\操作系统原理\实验3\连接成功.jpg)

   + win11系统下（我是win10系统，此情况照搬老师的实验手册）

     打开控制面板找到网络和Internet选项，点击高级网络设置，在wifi的更多适配器选项中点击编辑，在 共享选项卡上选中“允许其他网络用户通过此计算机的Internet连接来连接”选项。

     ![image-20240501220905383](C:\Users\123\AppData\Roaming\Typora\typora-user-images\image-20240501220905383.png)

     ![image-20240501220913796](C:\Users\123\AppData\Roaming\Typora\typora-user-images\image-20240501220913796.png)

4. 安装所需组件

   + 安装yum包管理器

     ```shell
     dnf install yum
     ```

   + 安装必需的工具unzip，tar，lrzsz，gcc，make，flex，bison，openssl-devel，perl

     ```shell
     yum -y install unzip tar lrzsz gcc make flex bison openssl-devel perl
     ```

####  openEuler内核编译与安装

1. 备份boot目录以防后续步骤更新内核失败

   ```shell
   tar czvf boot_origin.tgz /boot/
   ```

   **使用filezilla连接树莓派，然后将其下载到本地**

2. 获取内核源码并解压

   https://gitee.com/openeuler/raspberrypi-kernel/tree/openEuler-22.03-LTS-SP2/

   使用filezilla将内核源码压缩包上传到树莓派，在树莓派中进行解压和编译操作

   ```shell
   # 解压
   unzip raspberrypi-kernel-openEuler-22.03-LTS-SP2.zip
   #重命名
   mkdir -p /root/kernel
   mv /root/raspberrypi-kernel-openEuler-22.03-LTS-SP2/* /root/kernel
   rm -rf /root/raspberrypi-kernel-openEuler-22.03-LTS-SP2
   ```

3. 编译内核

   1. 加载默认配置

      ```shell
      cd kernel
      make bcm2711_defconfig
      ```

      ![image-20240501221527237](C:\Users\123\AppData\Roaming\Typora\typora-user-images\image-20240501221527237.png)

   2. 内核编译（此步骤我大概耗时三个小时）

      ```shell
      make ARCH=arm64 -j4
      ```

   3. 编译内核模块（此步骤我大概耗时十分钟）

      ```shell
      mkdir ../output
      make INSTALL_MOD_PATH=../output/modules_install
      ```

      完成23两个步骤后如图：

      ![内核编译完成](D:\李梓涵\2024春季学期\操作系统原理\实验3\内核编译完成.jpg)

4. 切换内核

   + 查看当前内核版本

     ```shell
     uname -a
     ```

     ![oldKernel](D:\李梓涵\2024春季学期\操作系统原理\实验3\oldKernel.png)

   + 将内核放进引导

     ```shell
     cp arch/arm64/boot/Image /boot/kernel8.img
     ```

   + 将设备树文件放进引导

     ```shell
     cp arch/arm64/boot/dts/broadcom/*.dtb /boot/
     cp arch/arm64/boot/dts/overlays/*.dtb* /boot/overlays/
     cp arch/arm64/boot/dts/overlays/README /boot/overlays/
     ```

   + 重启系统

     ```shell
     reboot
     ```

     **注意：**如果此时树莓派连接wifi，则重启系统后wifi连接会丢失，需要重新使用网线进行连接，重复之前的操作进行设置，再次使用ssh连接好树莓派后，持久化写入wifi设置

     + 首先查看是否有wlan0

       使用下面的明林临时写入

       ```shell
       insmod ~/kernel/net/rfkill/rfkill.ko
       insmod ~/kernel/drivers/net/wireless/broadcom/brcm80211/brcmutil/brcmutil.ko
       insmod ~/kernel/net/wireless/cfg80211.ko
       insmod ~/kernel/drivers/net/wireless/broadcom/brcm80211/brcmfmac/brcmfmac.ko
       ```

       然后查看是否有wlan0

       ```shell
       ifconfig
       ```

     + 持久化写入wifi设置

       编辑rc.local文件，将上面的insmod命令写入文件中

       ```shell
       vim /etc/rc.local
       ```

       ```shell
       ```rc.local文件中，在最后添加```
       insmod ~/kernel/net/rfkill/rfkill.ko
       insmod ~/kernel/drivers/net/wireless/broadcom/brcm80211/brcmutil/brcmutil.ko
       insmod ~/kernel/net/wireless/cfg80211.ko
       insmod ~/kernel/drivers/net/wireless/broadcom/brcm80211/brcmfmac/brcmfmac.ko
       ```

       然后使用`reboot`重启树莓派

   + 查看新的内核版本

     仍然使用`uname -a`，版本号如下即为成功

     ![newKernel](D:\李梓涵\2024春季学期\操作系统原理\实验3\newKernel.png)

     至此，内核编译完成。

#### 内核编程入门示例

1. 内核编程的基本流程

   + 准备工作

     ```shell
     ```要在root文件夹下新建一项目文件夹```
     mkdir -p /root/exp0/
     # 新建.c文件和Makefile文件
     touch /root/exp0/hello_world.c
     touch /root/exp0/Makefile
     ```

   + 编译运行命令

     ```shell
     # 编译
     make
     # 加载编译完成的内核模块（运行程序），在后边跟上对应参数的输入
     insmod hello_world.ko guy="Li" year=2024
     # 查看加载结果
     lsmod | grep hello_world
     # 卸载内核模块
     rmmod hello_world
     # 查看输出文件，查看结果
     dmesg | tail -n4
     ```

   + 查看模块参数信息的指令

     ```shell
     modinfo hello_world.ko
     ```

     ![modinfo](D:\李梓涵\2024春季学期\操作系统原理\实验3\modinfo.png)

2. 内核编程实例以及部分解释

   + hello_world.c

     ```c
     #include <linux/module.h>
     MODULE_LICENSE("GPL");	// 声明模块的许可证，缺少此声明会导致内核污染的警告
     static char* guy = "Kernel"; // 声明并初始化变量
     // 调用接受命令行参数的函数，传入参数名称，参数类型，用户使用参数的权限设置
     module_param(guy, charp, 0644); 
     // 描述驱动模块的参数信息，记录在.ko文件中，方便用户查看
     MODULE_PARM_DESC(guy, "char* param\n");
     static int year = 2021;
     module_param(year, int, 0644);
     MODULE_PARM_DESC(year, "int param\n");
     void hello_world(char* guy, int year) // 正常定义函数
     {
         // 注意此处输出使用的是printk函数
         printk(KERN_ALERT "Hello, %s, %d!\n", guy, year);
     }
     // __init是宏定义，告知编译器函数放在特定区域，将其标记为初始化函数，仅在模块初始化时调用，模块装在后即卸载，释放内存
     int __init hello_init(void)
     {
         printk(KERN_ALERT "Init module.\n");
         hello_world(guy, year);
         return 0;
     }
     // __exit类似__init，只不过视在模块支持无效时才将其后定义的变量或函数丢掉
     void __exit hello_exit(void)
     {
         printk(KERN_ALERT "Exit module.\n");
     }
     /*
     	module_init与module_exit类似，第一个规定了模块的入口函数，内核启动时或模块插入时调用；
     	第二个规定出口函数，当驱动程序是模块时，使用命令rmmod时调用该函数，同时使用cleanup_module包装驱动程序清理代码。如果驱动程序静态编译到内核中，则不起作用。
     	二者均唯一
     */
     module_init(hello_init);
     module_exit(hello_exit);
     ```

     

   + Makefile

     编译Linux内核模块的Makefile文件略有不同，先确定是否在内核构建环境中余小宁，如果不是就设置一些变量并调用内核的make命令；如果是就获取当前目录路径用于make编译。

     ```makefile
     # 通过KERNELRELEASE变量判断是否位于内核环境，如果有值说明可能在内核中
     ifneq ($(KERNELRELEASE),)
     	# 告知内核要构建一个名为hello_world的模块
     	obj-m := hello_world.o
     else
     	# 不在内核的构建系统中
     	# 对KERNELDIR设置内核的默认路径/root/kernel
     	KERNELDIR ?=/root/kernel
     	# 获取当前目录位置，即代码文件所在路径
     	PWD := $(shell pwd)
     default:
     	# 规定make命令的默认行为
     	# 调用make命令，-C $(KERNELDIR)指定了内核源代码目录，M=$(PWD)告诉内核的make系统模块源代码位于当前目录，modules是内核构建模块的目标。
     	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
     endif
     # 定义了一个名为clean的伪目标，.PHONY表示这个目标不是一个文件，而是一个执行特定命令的规则
     .PHONY:clean
     clean:
     	-rm *.mod.c *.o *.order *.symvers *.ko
     
     ```

     