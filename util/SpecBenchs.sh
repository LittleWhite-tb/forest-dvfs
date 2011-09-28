#! /bin/bash

RUN=$(date "+%d-%m_%T")



echo $RUN

mkdir $RUN

cd $RUN



#BENCHS=" 400.perlbench  435.gromacs    454.calculix    471.omnetpp 401.bzip2      436.cactusADM  456.hmmer       473.astar 403.gcc        437.leslie3d   458.sjeng       481.wrf 410.bwaves     444.namd       459.GemsFDTD    482.sphinx3 416.gamess     445.gobmk      462.libquantum  483.xalancbmk 429.mcf        447.dealII     464.h264ref     998.specrand 433.milc       450.soplex     465.tonto       999.specrand 434.zeusmp     453.povray     470.lbm        "
BENCHS=" 400.perlbench  435.gromacs    454.calculix 401.bzip2 444.namd  416.gamess     445.gobmk      462.libquantum   453.povray     470.lbm "





for i in $BENCHS; do

echo "Running the $i benchmark now"
mkdir $i
cd $i
$@ $i
cd ..
done


