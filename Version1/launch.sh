#!/bin/bash

#Need to check we are root


if [ "$(id -u)" != "0" ]; then
echo “Sudo make me a sandwich....” 2>&1
exit 1
fi

#choose which microbench to launch


CURRENT_DIR=$(pwd)

#add papi to your library path
LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
#load the powermeter shared library into the loader path (must be done while in sudo)
LD_LIBRARY_PATH="$CURRENT_DIR/../power/esrv:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH

echo "Changed ld_libary_path to $LD_LIBRARY_PATH"

#making a fresh binary
TARGET=$1
make clean
make



#getting the number of cores on the system
NUMCORESTEMP=$(cat /proc/cpuinfo | grep "processor" | wc -l)
NUMCORES=$(($NUMCORESTEMP))
#used to be $NUMCORESTEMP-1



NUMFREQS=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies | wc -w)


#Get the specific frequencies
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

#run the experiment!
echo "Running the benchmark on $NUMCORES cores"




#:<<COMMENTINGSOME
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
		./$TARGET --c=$i --r --m --i=1000 &
		PID[$i]=$!

		while [ ! -f /tmp/rest ]
		do 
		echo waiting > /dev/null
		done

	done
	#sync up cores before running next frequency
	for ((i=0;i<NUMCORES;i++))

	do

		wait "${PID[${i}]}"

	done

	rm /tmp/rest

done


#Frequencies scanned... now confirm all rest configurations



echo "Running experiment using REST-TOGETHER-UTOPIA setting in memory bound state"

make clean
make CFLAGS=-DORACLE_MODE-DCHANGETOGETHER

#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --m --i=1000 &
while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

PID[$i]=$!

done

rm /tmp/rest

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

./timing.sh $k
rm ./frequency_dump.txt

echo "Running experiment using REST-SEPARATE-UTOPIA setting in memory bound state"

make clean
make CFLAGS=-DORACLE_MODE

#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --m --i=1000 &
while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

PID[$i]=$!

done

rm /tmp/rest

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

./timing.sh $k
rm ./frequency_dump.txt

echo "Running experiment using REST-Profiler setting in memory bound state"

make clean all

#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --m --i=1000 &
while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

PID[$i]=$!

done

rm /tmp/rest

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"


done







echo "Doing Computebound only loop on all frequencies to characterize machine"

for ((j=0;j<NUMFREQS;j++))
do
	for ((i=0;i<NUMCORES;i++))

	do
		echo userspace > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
		echo ${FREQS[${j}]} > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_setspeed
		

	done

	rm /tmp/rest

	echo "Frequency set to ${FREQS[$j]}"
	for ((i=0;i<NUMCORES;i++))

	do
		./"$TARGET" --c=$i --r --p --i=1000 &
		PID[$i]=$!
		while [ ! -f /tmp/rest ]
		do 
		echo waiting > /dev/null
		done

	done
	#sync up cores before running next frequency
	for ((i=0;i<NUMCORES;i++))

	do

		wait "${PID[${i}]}"

	done

	rm /tmp/rest

done


#Frequencies scanned... now confirm all rest configurations

echo "Running experiment using REST-TOGETHER-UTOPIA setting in cpu bound state"

make clean
make CFLAGS=-DORACLE_MODE-DCHANGETOGETHER

#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --p --i=1000 &
while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

PID[$i]=$!

done

rm /tmp/rest

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

./timing.sh $k
rm ./frequency_dump.txt

echo "Running experiment using REST-SEPARATE-UTOPIA setting in cpu bound state"

make clean
make CFLAGS=-DORACLE_MODE

#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --p --i=1000 &
while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

PID[$i]=$!

done

rm /tmp/rest

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

./timing.sh $k
rm ./frequency_dump.txt

echo "Running experiment using REST-Profiler setting in cpu bound state"

make clean all

#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --p --i=1000 &
while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

PID[$i]=$!

done

rm /tmp/rest

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"


done

#COMMENTINGSOME



echo "Compute Bound and Memory Bound testing complete!"



#NOW we scan across different intervals

#setup which intervals we will test
INTERVALS=( .1 1 2 4 6 8 16 32 64 128)




for k in "${INTERVALS[@]}"
do

echo "Executing interval $k"

:<<COMMENTINGSOME
#running the programs... they will pin themselves to a core


for ((j=0;j<NUMFREQS;j++))
do

	for ((i=0;i<NUMCORES;i++))

	do
		echo userspace > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
		echo ${FREQS[${j}]} > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_setspeed
		

	done

	echo "Frequency set to ${FREQS[$j]} userspace setting for interval $k"
	for ((i=0;i<NUMCORES;i++))

	do
		./$TARGET --c=$i --r --i=$k &
		PID[$i]=$!
		while [ ! -f /tmp/rest ]
		do 
		echo waiting > /dev/null
		done

	done
	#sync up cores before running next frequency
	for ((i=0;i<NUMCORES;i++))

	do

		wait "${PID[${i}]}"

	done

	rm /tmp/rest

done




echo "Running experiment using performance setting using $k ms intervals"
#setting them all ondemand throttling
for ((i=0;i<NUMCORES;i++))

do
echo performance > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
done

rm /tmp/rest

#run the experiment
for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --r --i=$k &
PID[$i]=$!

while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

#echo "PID for core $i is ${PID[${i}]}"

done

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

rm /tmp/rest

echo "Running experiment using powersave setting using $k ms intervals"
#setting them all ondemand throttling
for ((i=0;i<NUMCORES;i++))

do
echo powersave > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
done


#run the experiment
for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --r --i=$k &
PID[$i]=$!


while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

#echo "PID for core $i is ${PID[${i}]}"

done

rm /tmp/rest

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

./$TARGET --c=$i --r --i=$k &
PID[$i]=$!

while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

#echo "PID for core $i is ${PID[${i}]}"

done

rm /tmp/rest

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

#COMMENTINGSOME

echo "Running experiment using REST-TOGETHER-UTOPIA setting at $k ms intervals"

make clean
make CFLAGS=-DORACLE_MODE-DCHANGETOGETHER

#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --i=$k &
while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

PID[$i]=$!

done

rm /tmp/rest

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

./timing.sh $k
rm ./frequency_dump.txt

echo "Running experiment using REST-SEPARATE-UTOPIA setting at $k ms intervals"

make clean
make CFLAGS=-DORACLE_MODE

#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --i=$k &
while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

PID[$i]=$!

done

rm /tmp/rest

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

./timing.sh $k
rm ./frequency_dump.txt

echo "Running experiment using REST-Profiler setting at $k ms intervals"

make clean all

#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./$TARGET --c=$i --i=$k &
while [ ! -f /tmp/rest ]
do 
echo waiting > /dev/null
done

PID[$i]=$!

done

rm /tmp/rest

#let experiment finish
for ((i=0;i<NUMCORES;i++))

do

#echo "wait ${PID[${i}]}"
wait "${PID[${i}]}"

done

./timing.sh $k
rm ./frequency_dump.txt

COMMENTINGSOME



done


