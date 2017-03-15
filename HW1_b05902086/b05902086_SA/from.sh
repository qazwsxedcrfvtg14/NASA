#!/bin/bash
last -i |
while read -r -a line
do
    if [[ ${line[0]} == "" ]]
    then
        break
    fi
    echo "${line[2]}"
done | 
sort |
uniq -c|
while read -r -a line
do
    geoiplookup "${line[1]}" | tr ":" "\n" |{
        read -r nation;
        read -r nation;
        if [[ ${nation:2:1} == ',' ]]
        then
            for((i=0;i<$((line[0]));i++))
            do 
            echo " $nation";
            done
        fi
    }
done | 
sort |
uniq -c|
sort -n
