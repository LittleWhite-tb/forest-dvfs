#!/bin/sh

#BENCHS=" h264ref  lbm  perlbench  bzip2  gcc  mcf  hmmer  sjeng  omnetpp  astar  xalancbmk  "
BENCHS="gcc mcf hmmer sjeng omnetpp astar xalancbmk"

for i in $BENCHS; do
	sudo mkdir /opt/rest/output/$i
	cd /opt/rest/output/$i
	sudo /opt/rest/util/sweeper.sh /opt/SPEC_BENCH/SPEC_2006V1_1/run_spec.sh --size=ref $i
done
