#!/bin/bash

echo "Available Speeds:" `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies`

if [ -z "$*" ]; then
  echo "CPU Frequency Missing!"
  echo "Usage: $0 <CPU> <CPU Frequency>"
  exit -1
fi

CPU=$1
SPEED=$2

echo -n "starting speed = "
cat /sys/devices/system/cpu/cpu${CPU}/cpufreq/scaling_cur_freq
sleep 3
echo -n "new speed = "
echo ${SPEED} > /sys/devices/system/cpu/cpu${CPU}/cpufreq/scaling_setspeed
cat /sys/devices/system/cpu/cpu${CPU}/cpufreq/scaling_cur_freq
#sleep 3
#echo 2133000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed
#echo -n "new speed = "
#cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
#sleep 3
#echo 2267000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed
#echo -n "new speed = "
#cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
