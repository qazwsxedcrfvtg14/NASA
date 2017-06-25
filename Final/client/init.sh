#!/bin/bash
host_name=192.168.1.203
yum update -y
yum install nfs-utils -y
yum install autofs
echo "/home/nfs /etc/autofs.config/auto.nfs" >> /etc/auto.master
mkdir /etc/autofs.config/
echo "mount -t nfs -ro,fg,soft nfs.config.server:/var/nfs.config /etc/autofs.config" >> /etc/rc.d/rc.local
chmod +x /etc/rc.d/rc.local
mount -t nfs -ro,fg,soft nfs.config.server:/var/nfs.config /etc/autofs.config
systemctl start autofs
systemctl enable autofs
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

