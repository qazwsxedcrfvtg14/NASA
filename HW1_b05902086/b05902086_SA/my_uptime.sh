#!/bin/bash
# Get localtime
zdump /etc/localtime | { 
    read -r -a tim; 
    printf " %s up " "${tim[4]}";
    }

# Get uptime
tr "." "\n" < /proc/uptime | { 
    read -r -a up;
    uptime=${up[0]};
    updays=$((uptime/86400));
    uphours=$(( (uptime-updays*86400)/3600 ));
    upmins=$(( (uptime-updays*86400-uphours*3600)/60 ));
    if [ "$updays" == "1" ]
    then
        printf "1 day, " $uphours;
    elif [ "$updays" != "0" ]
    then
        printf "%s days, " $updays;
    fi
    if [ $uphours == "0" ]
    then
        printf "%s min, " $upmins ;
    else
        printf "%2s:%02d, " $uphours $((upmins)) ;
    fi
    
    }

# Get logged-on users

who |wc -l|{
    read usr;
    if [ "$usr" == "1" ]
    then
        printf  "%2s user, " "$usr"
    else
        printf  "%2s users, " "$usr"
    fi
}

#echo users

# Get load average
{ 
read -r -a lod < /proc/loadavg; 
printf " load average: %s, %s, %s\n" "${lod[0]}" "${lod[1]}" "${lod[2]}";
}
#uptime