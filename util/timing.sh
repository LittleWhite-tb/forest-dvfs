#!/bin/bash



INTERVAL=$1

CORES=$2

NAME=$3

rm  Temp$INTERVAL.txt
for ((k=0;k<CORES;k++))
do

cat timing_dump_core$k.txt >> Temp$INTERVAL.txt
cat frequency_dump$k.txt >> Temp$INTERVAL.txt
sort -k1,1 Temp$INTERVAL.txt > ./Core$k.$NAME.Timing_Inspection_interval$INTERVAL.txt
rm  Temp$INTERVAL.txt

done



for ((k=0;k<CORES;k++))
do

cat Core$k.$NAME.Timing_Inspection_interval$INTERVAL.txt >>  Temp$INTERVAL.txt

done

sort -k1,1 Temp$INTERVAL.txt > ./$NAME.Timing_Inspection_interval$INTERVAL_aggregated.txt

rm Temp$INTERVAL.txt

rm frequency_dump*.txt
rm timing_dump_core*.txt

