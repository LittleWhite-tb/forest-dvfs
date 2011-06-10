#!/bin/bash

#Need to check we are root


if [ "$(id -u)" != "0" ]; then
echo “Sudo make me a sandwich....” 2>&1
exit 1
fi

#making a fresh binary
make


#getting the number of cores on the system
NUMCORES=$(cat /proc/cpuinfo | grep "processor" | wc -l)

echo "Running the benchmark on $NUMCORES cores"

#setting them all userspace throttling
for ((i=0;i<NUMCORES;i++))

do
echo userspace > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
done


#running the programs... they will pin themselves to a core

for ((i=0;i<NUMCORES;i++))

do

./Microbench1 -c$i &

done





