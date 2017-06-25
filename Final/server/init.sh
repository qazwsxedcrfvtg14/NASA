#!/bin/bash
host_name=nfs1.nfs.server
yum update -y
yum install nfs-utils -y
echo "mount -t nfs -ro,fg,soft nfs.config.server:/var/nfs.config /etc/autofs.config" >> /etc/rc.d/rc.local
mkdir /etc/autofs.config/
chmod +x /etc/rc.d/rc.local
mount -t nfs -ro,fg,soft nfs.config.server:/var/nfs.config /etc/autofs.config
mkdir -m 777 /var/nfs
mkdir -m 777 /var/nfs_backup
echo "/var/nfs 192.168.1.0/24(rw,sync,no_root_squash,no_all_squash)" >> /etc/exports
echo "/var/nfs_backup 192.168.1.0/24(rw,sync,no_root_squash,no_all_squash)" >> /etc/exports
systemctl enable rpcbind
systemctl enable nfs-server
systemctl enable nfs-lock
systemctl enable nfs-idmap
systemctl start rpcbind
systemctl start nfs-server
systemctl start nfs-lock
systemctl start nfs-idmap
firewall-cmd --permanent --zone=public --add-service=nfs
firewall-cmd --reload 
service_path=/usr/lib/systemd/system/balance.service
rm -f $service_path
echo "[Unit]" >> $service_path
echo "Description=My Script" >> $service_path
echo "[Service]" >> $service_path
echo "Type=simple" >> $service_path
echo "User=root" >> $service_path
echo "ExecStart=/etc/autofs.config/balance_service.sh $host_name" >> $service_path
echo "[Install]" >> $service_path
echo "WantedBy=multi-user.target" >> $service_path
systemctl daemon-reload
systemctl start balance
systemctl enable balance
