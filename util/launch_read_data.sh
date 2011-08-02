#!/bin/bash

FILENAME=$1

NAME=`echo "$FILENAME" | cut -d'.' -f1`

rm "$NAME.csv"

$(grep -v "gcc" $FILENAME > bigtemp.txt)

cp bigtemp.txt $FILENAME


NUMINTERVALS=$(grep -n "Executing" $FILENAME | cut -f1 -d: | wc -l)


echo "Number of intervals is $NUMINTERVALS"

INTERVALS=$(grep -n "intervals" $FILENAME | grep -v at | cut -f7 -d" " | uniq)

declare -a INTERVALLINES

for ((i=0;i<NUMINTERVALS;i++))

do

INTERVALS[$i]=$(grep -n "Executing" $FILENAME | cut -f3 -d" " | uniq | head -n$((i + 1)) | tail -n1 )
INTERVALLINES[$i]=$(grep -n "Executing" $FILENAME | cut -f1 -d":" | uniq | head -n$((i + 1)) | tail -n1 )

done

TOTALLINES=$(wc -l $FILENAME | cut -f1 -d" ")

INTERVALLINES[$NUMINTERVALS]=$TOTALLINES


FREQS=$(grep "Frequency set to" "$FILENAME" | awk {' print $4 '} |  sort -n | uniq | awk '{ printf "%s ", $0 }')

echo "Interval(ms), Energy, Time, Percent CPU, Percent Memory, Work Done, Frequency or Govenor" > $NAME.csv

for ((i=0;i<=NUMINTERVALS;i++))

do
#cut out the piece of the file we are looking at
if [[ $NUMINTERVALS -eq 0 ]]; then
cat $FILENAME > ./temp.txt
else
sed "${INTERVALLINES[$i]}, ${INTERVALLINES[$((i+1))]} !d" $FILENAME > ./temp.txt
fi



#find how many tests there are in this piece (usually the same)
NUMTESTS=$(grep "Total energy:" ./temp.txt | wc -l)

for ((j=0;j<NUMTESTS;j++))

do

grep -m$((j+1)) -B6 -A6 "Total energy:" ./temp.txt | tail -n13 > othertemp.txt



echo -n "${INTERVALS[$i]}, " >> $NAME.csv
echo -n "$(grep energy: othertemp.txt | cut -f3 -d" "), " >> $NAME.csv #energy
echo -n "$(grep time: othertemp.txt | tail -n1 | cut -f3 -d" "), " >> $NAME.csv #time
echo -n "$(grep Processor othertemp.txt | tail -n1 | cut -f1 -d" "), " >> $NAME.csv #cpu
echo -n "$(grep Memory othertemp.txt | head -n1 | cut -f1 -d" "), " >> $NAME.csv #mem
echo -n "$(grep transfers othertemp.txt |  tail -n1 | cut -f9 -d" "), " >> $NAME.csv #work

if [[ $(grep "Frequency set" othertemp.txt | wc -l) -eq 1 ]]; then
echo "$(grep set othertemp.txt | cut -f4 -d" "), " >> $NAME.csv #its a static Frequency so just give the number
elif [[ $(grep "Naive" othertemp.txt | wc -l) -eq 1 ]]; then
echo "Naive Profiler"  >> $NAME.csv 
elif [[ $(grep "Predictive" othertemp.txt | wc -l) -eq 1 ]]; then
echo "Predictive Profiler"  >> $NAME.csv 
elif [[ $(grep "powersave" othertemp.txt | wc -l) -eq 1 ]]; then
echo "Powersave"  >> $NAME.csv 
elif [[ $(grep "performance" othertemp.txt | wc -l) -eq 1 ]]; then
echo "Performance"  >> $NAME.csv 
elif [[ $(grep "REST-Profiler" othertemp.txt | wc -l) -eq 1 ]]; then
echo "Rest-Profiler" >> $NAME.csv
elif [[ $(grep "REST-TOGETHER-UTOPIA" othertemp.txt | wc -l) -eq 1 ]]; then
echo "Rest-Utopia-Together" >> $NAME.csv 
elif [[ $(grep "ondemand" othertemp.txt | wc -l) -eq 1 ]]; then
echo "OnDemand" >> $NAME.csv  
else
echo "Rest-Utopia-Separate"  >> $NAME.csv 
fi


done

done

rm othertemp.txt
rm bigtemp.txt
rm temp.txt



