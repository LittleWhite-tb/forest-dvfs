#!/bin/sh

BENCHS="milc gobmk"

#BENCHS=" h264ref  lbm  perlbench  bzip2  gcc  mcf  hmmer  sjeng  omnetpp  astar  xalancbmk  "
#BENCHS="gcc mcf hmmer sjeng omnetpp astar xalancbmk"

for i in $BENCHS
do
	mkdir /opt/rest_modifications/output/$i
	
	/opt/rest_modifications/util/sweeper.pl --freqrange=2 --freqid="0,1" --application=SPEC --output_folder=/opt/rest_modifications/output/$i /opt/SPEC_BENCH/SPEC_2006/run_spec.sh $i -- --size=ref
done
