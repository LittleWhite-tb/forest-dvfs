#!/bin/bash

echo "Available Speeds:" `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies`

if [ -z "$*" ]; then
  echo "CPU Frequency Missing!"
  echo "Usage: $0 <CPU Frequency>"
  exit -1
fi

cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
SPEED=$1
echo -n "starting speed = "
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
sleep 3
echo -n "new speed = "
echo ${SPEED} > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu1/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu2/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu3/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu4/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu5/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu6/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu7/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu8/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu9/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu10/cpufreq/scaling_setspeed
echo ${SPEED} > /sys/devices/system/cpu/cpu11/cpufreq/scaling_setspeed
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
