#!/bin/bash

#Need to check we are root


if [ "$(id -u)" != "0" ]; then
echo “Sudo make me a sandwich....” 2>&1
exit 1
fi

#making a fresh binary
make clean
make


#getting the number of cores on the system
NUMCORES=$(cat /proc/cpuinfo | grep "processor" | wc -l)

NUMFREQS=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies | wc -w)
#FREQS=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies)

#run the experiment
for ((i=0;i<NUMFREQS;i++))

do

FREQS[$i]=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies | cut -f$(($i+1)) -d" ")
 
done



echo "Number of cores is $NUMCORES and number of frequencies is $NUMFREQS"


#setting the sighandler to capture the children if we exit the script early
function on_controlc
{
	
	echo "Killing the processes of the experiment"

	for ((i=0;i<NUMCORES;i++))

	do

	echo "kill ${PID[${i}]}"

	kill "${PID[${i}]}"

	done

	#putting original cpufreq policies back in place
	for ((i=0;i<NUMCORES;i++))

	do
		echo ondemand > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
	done
	

	
	exit 0
}

#set this as sighandler

trap 'on_controlc' SIGINT


echo "Running the benchmark on $NUMCORES cores"

CURRENT_DIR=$(pwd)

#load the powermeter shared library into the loader path (must be done while in sudo)
LD_LIBRARY_PATH="$CURRENT_DIR/../power/esrv:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH

INTERVALS=( 10 20 30 40 50 100 150 300 500 1000 )


echo "Changed ld_libary_path to $LD_LIBRARY_PATH"

echo "Doing Memorybound only loop on all frequencies to characterize machine"

for ((j=0;j<NUMFREQS;j++))
do
	for ((i=0;i<NUMCORES;i++))

	do
		echo userspace > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
		echo ${FREQS[${j}]} > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_setspeed
		

	done

	echo "Frequency set to ${FREQS[$j]}"
	for ((i=0;i<NUMCORES;i++))

	do
		./Microbench1 --c=$i --r --m --i=1000 &
		PID[$i]=$!

	done
	#sync up cores before running next frequency
	for ((i=0;i<NUMCORES;i++))

	do

		wait "${PID[${i}]}"

	done

done

echo "Doing Computebound only loop on all frequencies to characterize machine"

for ((j=0;j<NUMFREQS;j++))
do
	for ((i=0;i<NUMCORES;i++))

	do
		echo userspace > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
		echo ${FREQS[${j}]} > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_setspeed
		

	done

	echo "Frequency set to ${FREQS[$j]}"
	for ((i=0;i<NUMCORES;i++))

	do
		./Microbench1 --c=$i --r --p --i=1000 &
		PID[$i]=$!

	done
	#sync up cores before running next frequency
	for ((i=0;i<NUMCORES;i++))

	do

		wait "${PID[${i}]}"

	done

done



for k in "${INTERVALS[@]}"
do


#running the programs... they will pin themselves to a core

echo "Sweeping all frequencies and govenors using $k ms intervals"  

for ((j=0;j<NUMFREQS;j++))
do
	for ((i=0;i<NUMCORES;i++))

	do
		echo userspace > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
		echo ${FREQS[${j}]} > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_setspeed
		

	done

	echo "Frequency set to ${FREQS[$j]}"
	for ((i=0;i<NUMCORES;i++))

	do
		./Microbench1 --c=$i --r --i=$k &
		PID[$i]=$!

	done
	#sync up cores before running next frequency
	for ((i=0;i<NUMCORES;i++))

	do

		wait "${PID[${i}]}"

	done

done




echo "Running experiment using performance setting using $k ms intervals"
#setting them all ondemand throttling
for ((i=0;i<NUMCORES;i++))

do
echo performance > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
done


#run the experiment
for ((i=0;i<NUMCORES;i++))

do

./Microbench1 --c=$i --r --i=$k &
PID[$i]=$!

#echo "PID for core $i is ${PID[${i}]}"

done

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

echo "Running experiment using powersave setting using $k ms intervals"
#setting them all ondemand throttling
for ((i=0;i<NUMCORES;i++))

do
echo powersave > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
done


#run the experiment
for ((i=0;i<NUMCORES;i++))

do

./Microbench1 --c=$i --r --i=$k &
PID[$i]=$!

#echo "PID for core $i is ${PID[${i}]}"

done

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done


echo "Running experiment using ondemand setting using $k ms intervals"
#setting them all ondemand throttling
for ((i=0;i<NUMCORES;i++))

do
echo ondemand > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
done


#run the experiment
for ((i=0;i<NUMCORES;i++))

do

./Microbench1 --c=$i --r --i=$k &
PID[$i]=$!

#echo "PID for core $i is ${PID[${i}]}"

done

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done


echo "Running experiment using possible REST setting at $k ms intervals"

#setting them all userspace throttling
for ((i=0;i<NUMCORES;i++))

do
echo userspace > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
done


#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./Microbench1 --c=$i --i=$k &
PID[$i]=$!

done

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

done


