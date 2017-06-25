#!/bin/bash


echo "0 4 * * * root /usr/local/bin/syncnfs "$1 >> /etc/crontab
