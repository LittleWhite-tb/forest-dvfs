#!/bin/sh
source /opt/intel/mpi-rt/4.0.2/bin64/mpivars.sh
source /opt/intel/composerxe/bin/compilervars.sh intel64
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/
ulimit -s unlimited

