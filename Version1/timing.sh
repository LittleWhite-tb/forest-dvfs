#!/bin/bash



INTERVAL=$1

FILES=$(ls *dump_core*.txt)

cat frequency_dump.txt > Temp$INTERVAL.txt

for k in "${FILES[@]}"

do

cat $k >> Temp$INTERVAL.txt

done

sort -k1,1 Temp$INTERVAL.txt > Timing_Inspection_interval$INTERVAL.txt
rm Temp$INTERVAL.txt

