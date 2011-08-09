#!/bin/bash

#Need to check we are root


if [ "$(id -u)" != "0" ]; then
echo “Sudo make me a sandwich....” 2>&1
exit 1
fi

source /opt/util/redo_sudo.rc

#need to make this some kind of command in the future
REST_PATH=$PWD/$(dirname $0)/../

CURRENT_DIR=$PWD

mkdir output
mkdir resume_data

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

function testFunc ()
{

if [ -e ./run_spec.sh ];
then

	BASENAME=$1
	microlaunch --basename $BASENAME --evallib /opt/microlaunch/power/timer.so --nbprocess=$NUMCORES --execname=$TOTAL_PATH --execargs "\" $@ \"" 

else

	BASENAME=$1
	shift 1
	microlaunch --basename $BASENAME --evallib /opt/microlaunch/power/timer.so --nbprocess=$NUMCORES --execname=$TOTAL_PATH --execargs "\" $@ \"" 
fi
	

}
	

cd $TARGET_PATH
if [ -e ./build_spec.sh ];
then

	./build_spec.sh #ok

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
	testFunc ${FREQS[$j]} $@ 
done




echo "Building for REST with ThreadedProfiler"

cd $TARGET_PATH
if [ -e ./build_spec.sh ];
then

	./build_spec.sh threaded all #ok

else

	make clean
	make CFLAGS=-DTHREAD_REST
fi
cd $CURRENT_DIR

testFunc threaded $@

exit

echo "Building for REST with VMAD"

cd $TARGET_PATH
if [ -e ./build_spec.sh ];
then

	./build_spec.sh vmad all #ok

else

	make clean
	make 
fi
cd $CURRENT_DIR

testFunc thread $@


