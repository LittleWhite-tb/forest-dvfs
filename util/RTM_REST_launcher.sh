#!/bin/bash

current_path=pwd
cd /opt/rest/Version1
make clean && make

cd /opt/rest/Version1/Exemple/RTM_test_kernel/
make clean && make
cd $current_path

runprog() 
{
	 microlaunch --basename test_RTM --output-dir RTM_test_output_$1 --metarepet 1 --repetition 1 --evallib "/opt/microlaunch/Libraries/power/timer.so;/opt/microlaunch/Libraries/timer/timer.so" --nbprocess=12 --execname=/opt/rest/Version1/Exemple/RTM_test_kernel/test_kernel
}

for i in `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies`; do

	freq1=0
	freq11=0
	cnt=0
	echo "* Selected frequency [ $i ]"

	while [ $cnt -lt 10 ]; do
	
		if [ $freq1 -ne $i -o $freq11 -ne $i ]; then
	
			echo "** Setting ondemand gov"
			sudo /opt/util/bin/set_all_ondemand_gov.sh
			echo "*** Verification [ `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor` ]"
			
			sleep 1

			# Run program using different frequency
			echo "** Setting userspace gov"
			sudo /opt/util/bin/set_userspace_gov.sh
			echo "*** Verification [ `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor` ]"
			sleep 1

			echo "** Setting the chosen freq"
			sudo /opt/util/bin/set_freq_ug.sh $i > /dev/null

			sleep 1
		fi

		#Now check
		"*** Verification [ `cat /sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq` vs `cat /sys/devices/system/cpu/cpu11/cpufreq/scaling_cur_freq` ]"
		freq1=`cat /sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq`
		freq11=`cat /sys/devices/system/cpu/cpu11/cpufreq/scaling_cur_freq`

		((cnt++))
	done
	
	runprog $i 
done


echo "** Setting ondemand gov"
sudo /opt/util/bin/set_all_ondemand_gov.sh
echo "*** Verification"
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
sleep 1

runprog onDemand
