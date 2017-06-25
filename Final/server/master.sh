#!/bin/bash
host_name=nfs1.nfs.server
echo "/var/nfs.config 192.168.1.0/24(ro,sync,no_root_squash,no_all_squash)" >> /etc/exports
mkdir /var/nfs.config
tar -zpxf nfs.config.tar.gz -C /
systemctl restart rpcbind
systemctl restart nfs-server
systemctl restart nfs-lock
systemctl restart nfs-idmap
