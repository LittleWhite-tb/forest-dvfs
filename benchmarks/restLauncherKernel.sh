#!/bin/bash

# Configuration
NBCORES=`grep CPU /proc/cpuinfo | wc -l`;
NFSPATH=/home/users/jphalimi/
LIBPATH=$NFSPATH/microlaunch/Libraries
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIBPATH/energy/esrv/:/opt/intel/lib/intel64/
repets=10
machine=`hostname`;

for r in my old 
do
	# Launch REST 2.0
	echo "Launching ${r}REST...";
	$NFSPATH/${r}Rest/localRest &
	restPid=$!
	sleep 1; # Let some time for REST to start...

	echo "${r}REST Launched (${restPid})";
	
	OUTPUT=results/$machine/microbenchs/${r}Rest/;
	mkdir -p $OUTPUT;

	kill -s USR1 $restPid

	for ((i=0;i<$repets;i++))
	do
		# Memory bound (movaps loads)
		$NFSPATH/microlaunch/microlaunch --kernelname "microbenchs/load.s" --nbprocess $NBCORES --repetition 50 --metarepetition 10 --startvector 40000000 --basename "load_$i" --info "raw;raw" --evallib "$LIBPATH/power_snb_msr/libraries/energy_msr_snb.so;$LIBPATH/timer/timer.so" --output-dir $OUTPUT

		kill -s USR1 $restPid;
	
		# CPU Bound (addpd)
		$NFSPATH/microlaunch/microlaunch --execname "microbenchs/addBenchAlex/testcpu2" --nbprocess $NBCORES --repetition 100 --metarepetition 10 --basename "add_$i"  --info "raw;raw" --evallib "$LIBPATH/power_snb_msr/libraries/energy_msr_snb.so;$LIBPATH/timer/timer.so" --output-dir $OUTPUT

		kill -s USR1 $restPid;

		# Memory bound (movaps stores)
		$NFSPATH/microlaunch/microlaunch --kernelname "microbenchs/store.s" --nbprocess $NBCORES --repetition 50 --metarepetition 10 --startvector 40000000 --basename "store_$i" --info "raw;raw" --evallib "$LIBPATH/power_snb_msr/libraries/energy_msr_snb.so;$LIBPATH/timer/timer.so" --output-dir $OUTPUT

		kill -s USR1 $restPid;
	done

	echo "Killing REST...";
	kill -s INT $restPid
	
	echo "Moving RESTlog...";
	mv RESTlog* $OUTPUT/
	clear;
done

