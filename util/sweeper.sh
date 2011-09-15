#!/bin/bash

#Need to check we are root


if [ "$(id -u)" != "0" ]; then
echo “Sudo make me a sandwich....” 2>&1
exit 1
fi

PID=0

#need to make this some kind of command in the future
REST_PATH=$(dirname $0)/../
source $REST_PATH/util/redo_sudo.rc


VMAD_PATH=$REST_PATH/../vmad

CURRENT_DIR=$PWD

mkdir output
mkdir resume_data

REST_OUTPUT=$CURRENT_DIR/output

export REST_OUTPUT

TOTAL_PATH=$1

TARGET_PATH=$(dirname $1)

TARGET=$(basename "$1")

shift 1

#make the rest library up to date
cd $REST_PATH/Version1

make clean
make

cd $CURRENT_DIR



POWER_PATH=$REST_PATH/power
echo "Power path for UpDown is $POWER_PATH"
#need to link esrv.so and also timer.so which is a directory above
POWER_PATH=$POWER_PATH:$POWER_PATH/esrv/
#add papi to your library path
LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
#load the powermeter shared library into the loader path (must be done while in sudo)
LD_LIBRARY_PATH="$POWER_PATH:$REST_PATH/Version1:$LD_LIBRARY_PATH"



export LD_LIBRARY_PATH


echo "Changed ld_libary_path to $LD_LIBRARY_PATH"
#making a fresh binary


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

#FUNCTION DECLARATIONS

function on_controlc
{
	echo "Control C Detected: Trying to kill the benchmarks..."
	echo "kill $PID"
	kill $PID
}

trap 'on_controlc' SIGINT


#otherwise we should be running on guedron
EVALLIB=/opt/microlaunch/Libraries/power/timer.so



function testFunc ()
{

	BASENAME=$1

if [ -e $TARGET_PATH/run_spec.sh ];
then
	echo "microlaunch  --basename $BASENAME --evallib $EVALLIB --nbprocess=$NUMCORES --execname=$TARGET_PATH/run_spec.sh --execargs \"$*\""

	microlaunch --basename $BASENAME --output-dir $REST_OUTPUT --metarepetition 1 --repetition 1 --evallib $EVALLIB --nbprocess=$NUMCORES --execname=$TARGET_PATH/run_spec.sh --execargs "$*"

else
	shift 1
	microlaunch --basename $BASENAME --output-dir $REST_OUTPUT --metarepetition 1  --repetition 1 --evallib $EVALLIB --nbprocess=$NUMCORES --execname=$TOTAL_PATH --execargs "$*" 
fi
	PID=$!

}
:<<commenting

echo "Building for REST with ThreadedProfiler"

cd $TARGET_PATH
if [ -e ./build_spec.sh ];
then

	./build_spec.sh -threaded $* #ok

else

	make clean
	make CFLAGS=-DTHREAD_REST
fi
cd $CURRENT_DIR
date >> threaded.start
testFunc -threaded $*
date>>threaded.stop


commenting


cd $TARGET_PATH
if [ -e ./build_spec.sh ];
then

	./build_spec.sh $*

else

	make clean
	make
fi
cd $CURRENT_DIR



#run the experiment!
echo "Running the benchmark on $NUMCORES cores"

for ((j=0;j<NUMFREQS;j++))
do

	for ((i=0;i<NUMCORES;i++))

	do
		echo userspace > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
		echo ${FREQS[${j}]} > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_setspeed
	done

	echo "Frequency set to ${FREQS[$j]} userspace setting"
	date >> ${FREQS[$j]}.start
	testFunc ${FREQS[$j]} $@ 
	date >> ${FREQS[$j]}.stop

done
echo "Using onDemand Governor"

for ((i=0;i<NUMCORES;i++))

do
	echo ondemand > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor
done
date>>ondemand.start
testFunc -ondemand $*
date>>ondemand.stop



exit


echo "Building for REST with VMAD"

cd $TARGET_PATH
if [ -e ./build_spec.sh ];
then

	./build_spec.sh -vmad all #ok

else

	make clean
	make 
fi
cd $CURRENT_DIR

testFunc thread $@


