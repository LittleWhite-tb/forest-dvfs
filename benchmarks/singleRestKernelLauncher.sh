#!/bin/bash

# Configuration
NBCORES=`grep CPU /proc/cpuinfo | wc -l`
MLPATH=$HOME/nfs/microlaunch
LIBPATH=$MLPATH/Libraries
repets=10
machine=`hostname`

# Launch REST 2.0
echo "Launching REST..."
../localRest performance &
restPid=$!
sleep 1 # Let some time for REST to start...

echo "$REST Launched (${restPid})"
	
OUTPUT=results/$machine/microbenchs
mkdir -p $OUTPUT

kill -s USR1 $restPid

for ((i=0;i<$repets;i++))
do
   # Memory bound (movaps loads)
	$MLPATH/microlaunch --kernelname "microbenchs/load.s" --nbprocess $NBCORES --repetition 50 --metarepetition 10 --startvector 40000000 --basename "load_$i" --info "raw;raw" --evallib "$LIBPATH/power_snb_msr/libraries/energy_msr_snb.so;$LIBPATH/timer/timer.so" --output-dir $OUTPUT
	
	kill -s USR1 $restPid
	
	# CPU Bound (addpd)
	$MLPATH/microlaunch --execname "microbenchs/addBenchAlex/testcpu2" --nbprocess $NBCORES --repetition 100 --metarepetition 10 --basename "add_$i"  --info "raw;raw" --evallib "$LIBPATH/power_snb_msr/libraries/energy_msr_snb.so;$LIBPATH/timer/timer.so" --output-dir $OUTPUT

	kill -s USR1 $restPid

	# Memory bound (movaps stores)
	$MLPATH/microlaunch --kernelname "microbenchs/store.s" --nbprocess $NBCORES --repetition 50 --metarepetition 10 --startvector 40000000 --basename "store_$i" --info "raw;raw" --evallib "$LIBPATH/power_snb_msr/libraries/energy_msr_snb.so;$LIBPATH/timer/timer.so" --output-dir $OUTPUT

	kill -s USR1 $restPid
done

echo "Killing REST..."
kill -s INT $restPid
	
echo "Moving RESTlog..."
mv RESTlog* $OUTPUT/

