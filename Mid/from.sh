#!/bin/bash
grep ": NOTICE "<fail2ban.log |grep "Ban" |
#cat out.txt|
awk '{print $8}'|
sort |
uniq -c|
awk '{print $2}'|
while read  line
do
    geoiplookup "${line}" | tr ":" "\n" |{
        read nation;
        read nation;
        echo " $nation";
    }
done |
sort |
uniq -c|
sort -n -r
