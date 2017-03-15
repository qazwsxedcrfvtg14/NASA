#!/bin/bash
#awk -F "</*td>|</*tr>|</*TR>|</*TD>" '/<\/*[tT][rdRD]>.*/ {print "_"$2"_"$4"_"$6"_"$8"_"$14"_" }' nol.html.txt 
#awk -F "</*td>|</*tr>|</*TR>|</*TD>" '/<\/*[tT][rdRD]>.*[A-Z][A-Z]/ {print $1,$2,$3,$4,$5, $7 }' nol.html.txt 
ignore_re="Seminar|Special +Project|Thesis|專題研究|專題討論|碩士論文|博士論文"
ok=0
gmode=0
dmode=0
file=0
fil_arg=0
if [[ $# == 0 ]];then
    echo "Missing input file"
    exit
fi
for arg in "$@"
do 
    #echo $arg;
    if [[ $fil_arg == 1 ]];then
        file=$arg
        fil_arg=0
        ok=1
    elif [[ ${arg:0:1} == "-" ]];then
        len=${#arg}
        for ((i=1;i<len;i++));do
            now=${arg:i:1}
            if [[ $now == "g" ]];then 
                gmode=1
            elif [[ $now == "d" ]];then
                dmode=1
            elif [[ $now == "h" ]];then 
                echo "Usage nol_parser.sh [OPTION]..."
                echo "  -f    append filename behind this option"
                echo "  -g    exclude courses whose name contains \"those\" strings."
                echo "  -h    display this help and exit"
                echo "  -d    don't output console control character"
                exit
            elif [[ $now == "f" ]];then
                if [[ $ok == 1 ]];then
                    echo "$0: Do not input multiple files!"
                    exit
                fi
                fil_arg=1
            else
                echo "$0: invalid option $now"
                exit
            fi
        done
    else
        echo "$0: invalid argument $arg"
        exit
    fi
done
if [[ $fil_arg == 1 ]];then
    echo "$0: Where is the file?"
    exit
fi
if [[ $ok == 0 ]];then
    echo "Missing input file"
    exit
fi
if ! [[ -f "$file" ]];then
	echo "Input file not found"
    exit
fi
#echo $gmode
#echo $file
begin=0
tr -d "\n"< "$file" |
awk -F "CCCCCC" '/CCCCCC/ {print $3 }'|
sed "s/<\/tr>/<\/tr>\\n/g" |
awk -F "</*td>|</*tr>|</*TR>|</*TD>" '/<\/*[tT][rdRD]>.*/ {print $10"$"$20"$"$24 }'|
#exit
#awk -F "</*td>|</*tr>|</*TR>|</*TD>" '/<\/*[tT][rdRD]>.*/ {print $10"$"$20"$"$24 }' $file |
while read -r line;do
    #echo $line;
    if [[ $line == "$""$" ]];then
        continue
    fi
    if [[ $begin == 1 ]];then
        echo "$line";
    fi
    begin=1
done |
#sed "s/<.*>//" |
tr "$" "\n"|
sed "s/<\/[Aa]>//g" |
sed "s/<[Aa].*>//g" |

while read -r line1; do
    read -r line2
    read -r line3
    cancelled="images/cancelled.jpg"
    if test "${line1#*$cancelled}" == "$line1"
    then
        echo "$line1"
        echo "$line2"
        echo "$line3"
    fi
done |


sed "s/<.*>//g"|
sed "s/\&.*;//g"|
sed "s/\&nbsp;//g"|
while read -r line1; do
    read -r line2
    read -r line3
    if [[ $gmode == 1 ]];then
        if [[ $line1 =~ $ignore_re ]];then 
            continue
        fi
    fi
    line3=$(echo "$line3" | sed -e "s/(.*)//g" -e "s/&nbsp;//g")
    if [[ $line1 == "" ]];then
        line1="N/A"
    fi
    if [[ $line2 == "" ]];then
        line2="N/A"
    fi
    if [[ $line3 == "" ]];then
        line3="N/A"
    else
        line3=${line3:0:3}" "${line3:3}
    fi
    #printf "%50s%20s%10s\n" $line1 $line2 $line3
    #printf "%s%s%s\n" $line1 $line2 $line3
    if [[ $dmode == 1 ]];then
        printf "%s\\033[100D\\033[50C\t%s\\033[100D\\033[70C\t\t%s\n" "$line1" "$line2" "$line3"
    else
        printf "%-50s\t%-20s\t%-10s\n" "$line1" "$line2" "$line3"
    fi
done 