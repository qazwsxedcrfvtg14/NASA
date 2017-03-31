# begin
```
fdisk /dev/sda
o
n
p
1
<default>
+1G
n
p
2
<default>
+2G
n
p
3
<default>
+6G
n
p
<default>
<default>
t
1
ef
t
2
82
t
3
83
t
4
8e
w

#/dev/sda1 ef 1G /boot
#/dev/sda2 82 2G /swap
#/dev/sda3 83 6G /
#/dev/sda4 8e 11G
#    /dev/VolGroup00/lvolvar 4G /var
#    /dev/VolGroup00/lvolhome 4G /home

lvmdiskscan
pvcreate /dev/sda4
vgcreate VolGroup00 /dev/sda4
vgdisplay
lvcreate -L 4G VolGroup00 -n lvolvar
lvcreate -L 4G VolGroup00 -n lvolhome
lvdisplay
mkfs.ext4 /dev/VolGroup00/lvolvar
mkfs.ext4 /dev/VolGroup00/lvolhome
tune2fs -m 10 /dev/VolGroup00/lvolvar
mkfs.fat -F32 /dev/sda1
mkswap /dev/sda2
mkfs.ext4 /dev/sda3

mount /dev/sda3 /mnt
swapon /dev/sda2
mkdir /mnt/boot
mount /dev/sda1 /mnt/boot
mkdir /mnt/var
mount /dev/VolGroup00/lvolvar /mnt/var
mkdir /mnt/home
mount /dev/VolGroup00/lvolhome /mnt/home
lsblk -f

pacstrap /mnt base
```
# init
```
genfstab -U /mnt >> /mnt/etc/fstab
arch-chroot /mnt
ln -sf /usr/share/zoneinfo/Asia/Taipei /etc/localtime
hwclock --systohc
vi /etc/locale.gen #uncommit en_US.UTF-8 UTF-8
locale-gen
echo "LANG=en_US.UTF-8" > /etc/locale.conf
echo "joe59491_arch" > /etc/hostname
echo "127.0.0.1 joe59491_arch.localdomain joe59491_arch" >> /etc/hosts
mkinitcpio -p linux
passwd
```
# before install grub
```
pacman -Syu
pacman -S grub
```
# after pacman grub
```
pacman -S parted
parted /dev/sda print
pacman -S efibootmgr
grub-install /dev/sda
grub-mkconfig -o /boot/grub/grub.cfg
exit
reboot
#login~
```
# after boot with MBR
```
reboot

# Turn on the EFI boot function
# boot in with iso

mount /dev/sda3 /mnt
swapon /dev/sda2
mount /dev/sda1 /mnt/boot
mount /dev/VolGroup00/lvolvar /mnt/var
mount /dev/VolGroup00/lvolhome /mnt/home
grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=grub # grub-install 把grub的程式寫入/boot裡面
# --target=x86_64-efi 將目標設定為 32/64 位元的 EFI 機器
# --efi-directory=/boot 設定 efi directory 的位置
# --bootloader-id=grub 設定 bootloader 的 id 為 grub
# EFI 是一個可以讓電腦的開機過程可以實現更多功能的一種介面
# EFI 開機的話需要先把開機磁區的label設定成 ef(EFI system partition) 並且格式化成 Fat32 
# 然後 UEFI 開機的時候就會去找磁碟中符合上面規定的磁區，然後從裡面開機
```
# after grub-install
```
grub-mkconfig -o /boot/grub/grub.cfg # grub-mkconfig 把啟動的過城中需要config建立好放在/boot/grub/grub.cfg
```
# after grub-mkconfig
```
reboot
#login~
ip link
cp /etc/netctl/examples/ethernet-dhcp /etc/netctl/
vi /etc/netctl/ethernet-dhcp #change eth0 to ens33
netctl list
netctl start ethernet-dhcp
pacman -S vim
```
# after connecting to network
```
df
lvresize -l +256 /dev/VolGroup00/lvolhome #increase 1GB
resize2fs /dev/VolGroup00/lvolhome
df
```
# after extend /home
vmware image:
https://csientuedutw-my.sharepoint.com/personal/b05902086_csie_ntu_edu_tw/_layouts/15/guestaccess.aspx?docid=0d42ab6b39f3648ab85ce547230cb4dd3&authkey=AfLd_alGsWNNLIH5BhWzWFE
