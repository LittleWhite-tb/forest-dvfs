#!/bin/bash

mic=mic0
localFolder=FoRESTOnMIC

mkdir $localFolder
mkdir $localFolder/offline
mkdir $localFolder/offline/lPowerProbe

cp FoREST $localFolder
cp config.cfg $localFolder
cp offline/add $localFolder/offline
cp offline/usefulFreqs.py $localFolder/offline/usefulFreqs.py
cp offline/lPowerProbe/lPowerProbe $localFolder/offline/lPowerProbe
cp -r offline/lPowerProbe/probes $localFolder/offline/lPowerProbe
cp -r offline/PythonForMIC/_binaries $localFolder/offline

# Copy localFolder on the mic
scp -r $localFolder $mic:~/

