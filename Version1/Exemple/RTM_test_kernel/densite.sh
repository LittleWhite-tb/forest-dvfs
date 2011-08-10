#!/bin/sh

echo $#
if [ $# -ne 3 ]
then
echo "usage : densite.sh <file> <limit size>"
exit 0
fi

for i in `seq 10000 10000 $2`
do
	echo "damping `head -i $2 | tail -100000 | grep 2 | wc -l`"
	head "inner `-i $2 | tail -100000 | grep 1 | wc -l`"
done
