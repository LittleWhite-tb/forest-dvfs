#!/bin/sh

BENCHS1="gcc" 
BENCHS2="cactusADM namd"
BENCHS3="soplex povray calculix"
BENCHS4="hmmer sjeng libquantum"
BENCHS5="h264ref lbm omnetpp astar"
BENCHS6="sphinx3 xalancbmk"

#for 6 proc : bzip2, milc, cactusADM
#tres gourmant : mcf

#soucis : gamess, GemsFDTD (ne marche que pour rest)

#BENCHS="perlbench wrf bwaves gamess zeusmp leslie3d GemsFDTD tonto"
BENCHS="run_spec/gcc/run_gcc.sh"

for i in $BENCHS
do
	mkdir /opt/rest_modifications/output/$i
	
	/opt/rest_modifications/util/sweeper.pl --nb_proc=12 --freqrange="restonly" --application=SPEC --output_folder=/opt/rest_modifications/output/$i /opt/SPEC_BENCH/SPEC_2006/run_spec.sh $i -- --size=ref #--freqrange=2 --freqid="0,1"  
done
