#!/bin/bash
/etc/autofs.config/service $1
#while /usr/local/bin/inotifywait -e close_write,moved_to,create /etc/autofs.config/change.txt; do
#/etc/autofs.config/service $1
#done
