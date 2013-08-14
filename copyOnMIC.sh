#!/bin/bash

mic=mic0
localFolder=FoRESTOnMIC

mkdir $localFolder
mkdir $localFolder/offline
mkdir $localFolder/offline/lPowerProbe

sudo cp FoREST $localFolder
sudo cp config.cfg $localFolder
sudo cp README_MIC $localFolder/README
sudo cp offline/add $localFolder/offline
sudo cp offline/usefulFreqs.py $localFolder/offline/usefulFreqs.py
sudo cp offline/lPowerProbe/lPowerProbe $localFolder/offline/lPowerProbe
sudo cp -r offline/lPowerProbe/probes $localFolder/offline/lPowerProbe
sudo cp -r offline/PythonForMIC/_binaries $localFolder/offline

# Copy localFolder on the mic
sudo scp -r $localFolder $mic:~/

