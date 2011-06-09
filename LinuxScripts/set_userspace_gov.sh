#!/bin/bash

NUMCORES=$(cat /proc/cpuinfo | grep "processor" | wc -l)

for ((i=0;i<NUMCORES;i++))

do

echo userspace > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor

done
