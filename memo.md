ssh -i "C:\Users\Yuandu\.ssh\id_rsa"

yunchuan@amd003.utah.cloudlab.us
yunchuan@amd021.utah.cloudlab.us

yunchuan@apt076.apt.emulab.net
yunchuan@apt083.apt.emulab.net

yunchuan@apt116.apt.emulab.net
yunchuan@apt108.apt.emulab.net

检查是否有rdma网卡：
lspci | grep -i ether
lspci | grep -i mell

检查是否识别了rdma设备：
ibv_devinfo

更新软件源列表：
sudo apt update

安装rdma工具包（先更新）：
sudo apt install infiniband-diags ibverbs-utils

检查系统是否启动了rdma设备：
ls /sys/class/infiniband/

检查系统内核是否加载模块：
lsmod | grep -i infiniband

检查网络接口：
ip link show

找到哪个网卡和mlx5_0对应：
ls /sys/class/infiniband/mlx4_0/device/net/

网卡<name>：

enp65s0f0np0

enp8s0d1

给网卡（<name>）分配IP：（两个node都要做）
sudo ip addr add 192.168.1.1/24 dev enp65s0f0np0
sudo ip link set enp65s0f0np0 up

sudo ip addr add 192.168.1.2/24 dev enp65s0f0np0
sudo ip link set enp65s0f0np0 up

ping测试：
ping 192.168.1.2

检测网卡是否物理连接了：
ethtool enp65s0f0np0

检测rdma数据传输：
ibv_rc_pingpong
ibv_rc_pingpong 192.168.1.1



步骤总结：

《初态》
1. 安装工具包

2. 检查网卡存在，确认网卡name

3. 检查网卡物理连接

4. 配置IP，发ping检测

5. pingpong指令测试rdma