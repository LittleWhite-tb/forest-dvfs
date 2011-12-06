#!/bin/bash 

export I_MPI_PIN=1
export I_MPI_PIN_PROCESSOR_LIST="allcores:map=scatter"
#export I_MPI_PIN_PROCESSOR_LIST="allcores:map=compact"
export I_MPI_PIN_MODE=mpd

mpdallexit >& /dev/null
mpdboot

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
#		((cnt++)) 
#	done 
#	mkdir RTM_MPI_$i 
#	REST_OUTPUT=/opt/rest_modifications/Version1/Exemple/RTM_MPI/RTM_MPI_$i 
#	export REST_OUTPUT 
#	date >> $REST_OUTPUT/thetime.start 
#	mpiexec -np 12 ./fd_iso
#	date >> $REST_OUTPUT/thetime.stop 
#done

	echo "** Setting ondemand gov"
	sudo /opt/util/bin/set_all_ondemand_gov.sh
	echo "*** Verification [ `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor` ]"
	sleep 1	

	echo "** Setting userspace gov"
        sudo /opt/util/bin/set_userspace_gov.sh
        echo "*** Verification [ `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor` ]"
        sleep 1

	i=Naive	
	mkdir RTM_MPI_$i
	REST_OUTPUT=/opt/rest_modifications/Version1/Exemple/RTM_MPI/RTM_MPI_$i
	export REST_OUTPUT

	date >> $REST_OUTPUT/thetime.start
	mpiexec -np 12 ./fd_iso
	date >> $REST_OUTPUT/thetime.stop


mpdallexit
