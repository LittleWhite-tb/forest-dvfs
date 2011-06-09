#!/bin/bash

#echo "Available Speeds:" `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies`

if [ -z "$*" ]; then
  echo "CPU Frequency Missing!"
  echo "Usage: $0 <CPU Frequency>"
  exit -1
fi

#cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
SPEED=$1
#echo -n "starting speed = "
#cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
#echo -n "new speed = "

#./set_userspace_gov.sh  changing frequencies can only have if we are in userspace... so right now this is an insurance policy...


NUMCORES=$(cat /proc/cpuinfo | grep "processor" | wc -l)

for ((i=0;i<NUMCORES;i++))

do

echo ${SPEED} > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_setspeed

done

#cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
