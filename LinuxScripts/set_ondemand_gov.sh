#!/bin/bash

echo ondemand > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo ondemand > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
echo ondemand > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
echo ondemand > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
echo ondemand > /sys/devices/system/cpu/cpu4/cpufreq/scaling_governor
echo ondemand > /sys/devices/system/cpu/cpu5/cpufreq/scaling_governor
