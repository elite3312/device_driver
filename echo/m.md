# _

```sh
sudo apt update
sudo apt install build-essential raspberrypi-kernel-headers

# create driver directory
mkdir -p ~/git_repos/driver
scp echo_driver.c Makefile perry@perry:~/git_repos/driver
cd ~/git_repos/driver
make

sudo insmod echo_driver.ko
dmesg | tail
```

```txt
perry@perry:~/git_repos/driver $ sudo insmod echo_driver.ko
dmesg | tail
[    5.569382] Bluetooth: MGMT ver 1.23
[    5.590186] NET: Registered PF_ALG protocol family
[    6.969665] bcmgenet fd580000.ethernet: configuring instance for external RGMII (RX delay)
[    6.972355] bcmgenet fd580000.ethernet eth0: Link is Down
[    6.987004] brcmfmac: brcmf_cfg80211_set_power_mgmt: power save enabled
[   10.583592] Bluetooth: RFCOMM TTY layer initialized
[   10.583621] Bluetooth: RFCOMM socket layer initialized
[   10.583640] Bluetooth: RFCOMM ver 1.11
[ 1410.020368] echo_driver: loading out-of-tree module taints kernel.
[ 1410.023915] Echo driver loaded with major 236
```

```sh
sudo mknod /dev/echo_driver c 236 0
sudo chmod 666 /dev/echo_driver
```
```sh
perry@perry:~/git_repos/driver $ echo "Hello Pi" > /dev/echo_driver
perry@perry:~/git_repos/driver $ cat /dev/echo_driver
Hello Pi
```
```sh
sudo rmmod echo_driver
make clean
```