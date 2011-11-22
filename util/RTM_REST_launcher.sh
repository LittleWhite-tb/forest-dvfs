#!/bin/bash

#to make shure that all the libraries are loaded
export LD_LIBRARY_PATH=/opt/rest_modifications/Version1/:/opt/microlaunch/Libraries/power/:/opt/microlaunch/Libraries/power/esrv/


#compiling REST to make shure we have the lastest version
current_path=`pwd`
cd /opt/rest_modifications/Version1
make clean && make

#compiling REST to make shure we have the lastest version
cd /opt/rest_modifications/Version1/Exemple/RTM_test_kernel/RTM
make clean && make

cd /opt/rest_modifications/Version1/Exemple/RTM_test_kernel/REST_RTM
make clean && make
cd $current_path

runprog() 
{
	 local restOrnot=$2
	 microlaunch --basename test_RTM --output-dir RTM_test_output_$1 --metarepet 1 --repetition 1 --evallib "/opt/microlaunch/Libraries/power/timer.so;/opt/microlaunch/Libraries/timer/timer.so" --nbprocess=12 --execname=/opt/rest_modifications/Version1/Exemple/RTM_test_kernel/$restOrnot/test_kernel
}

#run of RTM without REST and, any governor, to see the consumption of RTM on various frequencies
#for i in `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies`; do

#	freq1=0
#	freq11=0
#	cnt=0
#	echo ""
#	echo ""
#	echo "* Selected frequency [ $i ]"

#	while [ $cnt -lt 10 ]; do
	
#		if [ $freq1 -ne $i -o $freq11 -ne $i ]; then
#			echo "** Setting ondemand gov"
#			sudo /opt/util/bin/set_all_ondemand_gov.sh
#			echo "*** Verification [ `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor` ]"
			
#			sleep 1

			# Run program using different frequency
#			echo "** Setting userspace gov"
#			sudo /opt/util/bin/set_userspace_gov.sh
#			echo "*** Verification [ `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor` ]"
#			sleep 1

#			echo "** Setting the chosen freq"
#			sudo /opt/util/bin/set_freq_ug.sh $i > /dev/null
#			sleep 1
#		fi

		#Now check
#		freq1=`cat /sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq`
#		freq11=`cat /sys/devices/system/cpu/cpu11/cpufreq/scaling_cur_freq`

#		((cnt++))#
#	done
	
#	runprog $i RTM
#done

#run of RTM without REST but with a governor to see the awsomeness of it
#echo ""
#echo ""
#echo "** Setting ondemand gov"
#sudo /opt/util/bin/set_all_ondemand_gov.sh
#echo "*** Verification [ `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor` ]"
#sleep 1
#runprog onDemand RTM



#run of RTM with REST to see the awsomeness of it ++++++++++++++++++++++
echo ""
echo ""
echo "** Setting userspace gov"
sudo /opt/util/bin/set_userspace_gov.sh
echo "*** Verification [ `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor` ]"
sleep 1

#NaiveDM ***************************************************************
for i in 600 #`seq 600 1000 6600`
do
	REST_PADING=$i
	export REST_PADING

	mkdir naive_RTM_$REST_PADING
	REST_OUTPUT=`pwd`/naive_RTM_$REST_PADING
	echo "**** Rest output directory set to $REST_OUTPUT"
	export REST_OUTPUT

	REST_DM=naive_dm
	export REST_DM
	date >> $REST_OUTPUT/naive.start
	runprog Naive_$REST_PADING REST_RTM
	date >> $REST_OUTPUT/naive.stop
done
#***********************************************************************

#PredictiveDM **********************************************************
#mkdir predictive
#REST_OUTPUT=`pwd`/predictive_RTM
#echo "Rest output directory set to $REST_OUTPUT"
#export REST_OUTPUT

#REST_DM=predictive_dm
#export REST_DM

#date >> $REST_OUTPUT/predictive.start
#runprog Predictive REST_RTM
#date >> $REST_OUTPUT/predictive.stop
#***********************************************************************

#MarkovDM **************************************************************
#mkdir markov
#REST_OUTPUT=`pwd`/markov_RTM
#echo "Rest output directory set to $REST_OUTPUT"
#export REST_OUTPUT

#REST_DM=markov_dm
#export REST_DM

#date >> $REST_OUTPUT/markov.start
#runprog Markov REST_RTM
#date >> $REST_OUTPUT/markov.stop
#***********************************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

NUMCORES=$(cat /proc/cpuinfo | grep "processor" | wc -l)

for DM in `pwd`/naive_RTM_600 
do
	for file in `ls $DM| grep core_frequency_count`
	do
		if [ `cat $DM/$file | wc -l` -ne 0 -o `cat $DM/$file | wc -l` -ne 1 ]
		then
			echo Results from $file >> $DM/agregated_core_frequency_count.txt
			cat $DM/$file >> $DM/agregated_core_frequency_count.txt
		fi
	done
done
