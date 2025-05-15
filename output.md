yunchuan@node-0:~$ lspci | grep -i ether
01:00.0 Ethernet controller: Broadcom Inc. and subsidiaries BCM57414 NetXtreme-E 10Gb/25Gb RDMA Ethernet Controller (rev 01)        
01:00.1 Ethernet controller: Broadcom Inc. and subsidiaries BCM57414 NetXtreme-E 10Gb/25Gb RDMA Ethernet Controller (rev 01)        
41:00.0 Ethernet controller: Mellanox Technologies MT28800 Family [ConnectX-5 Ex]
41:00.1 Ethernet controller: Mellanox Technologies MT28800 Family [ConnectX-5 Ex]
c1:00.0 Ethernet controller: Broadcom Inc. and subsidiaries NetXtreme BCM5720 Gigabit Ethernet PCIe
c1:00.1 Ethernet controller: Broadcom Inc. and subsidiaries NetXtreme BCM5720 Gigabit Ethernet PCIe



yunchuan@node-0:~$ ibv_devinfo
hca_id: mlx5_0
        transport:                      InfiniBand (0)
        fw_ver:                         16.35.1012
        node_guid:                      1c34:da03:0041:caac
        sys_image_guid:                 1c34:da03:0041:caac
        vendor_id:                      0x02c9
        vendor_part_id:                 4121
        hw_ver:                         0x0
        board_id:                       DEL0000000004
        phys_port_cnt:                  1
                port:   1
                        state:                  PORT_DOWN (1)
                        max_mtu:                4096 (5)
                        active_mtu:             1024 (3)
                        sm_lid:                 0
                        port_lid:               0
                        port_lmc:               0x00
                        link_layer:             Ethernet

hca_id: mlx5_1
        transport:                      InfiniBand (0)
        fw_ver:                         16.35.1012
        node_guid:                      1c34:da03:0041:caad
        sys_image_guid:                 1c34:da03:0041:caac
        vendor_id:                      0x02c9
        vendor_part_id:                 4121
        hw_ver:                         0x0
        board_id:                       DEL0000000004
        phys_port_cnt:                  1
                port:   1
                        state:                  PORT_DOWN (1)
                        max_mtu:                4096 (5)
                        active_mtu:             1024 (3)
                        sm_lid:                 0
                        port_lid:               0
                        port_lmc:               0x00
                        link_layer:             Ethernet


yunchuan@node-0:~$ ip link show
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eno1: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether d0:8e:79:ba:01:10 brd ff:ff:ff:ff:ff:ff
    altname enp193s0f0
3: enp1s0f0np0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether b0:26:28:74:7c:a0 brd ff:ff:ff:ff:ff:ff
4: enp1s0f1np1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether b0:26:28:74:7c:a1 brd ff:ff:ff:ff:ff:ff
5: eno2: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether d0:8e:79:ba:01:11 brd ff:ff:ff:ff:ff:ff
    altname enp193s0f1
6: enp65s0f0np0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 1c:34:da:41:ca:ac brd ff:ff:ff:ff:ff:ff
7: enp65s0f1np1: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 1c:34:da:41:ca:ad brd ff:ff:ff:ff:ff:ff
yunchuan@node-0:~$ ls /sys/class/infiniband/mlx5_0/device/net/
enp65s0f0np0


