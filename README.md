FoREST - Reactive DVFS Control for Multicore Processors

FoREST is a new DVFS daemon that controls the CPU frequency based periodical cpu-intensity analysis.

# Hardware support
* Intel x86: SandyBridge, IvyBridge
* Intel Xeon Phi (2013)

# Pre-Compilation
This command is necessary to setup the submodule of the project.
```
   git submodule update --init
```

The submodule project is:
* [lPowerProbe](https://github.com/LittleWhite-tb/lpowerprobe)

FoREST also features third-party libraries:
* [libpfm](http://perfmon2.sourceforge.net/)
* [libglog](https://code.google.com/p/google-glog/)

# Compilation
```
   make [x86|mic]
   make [x86-debug|mic-debug]
```
Note: On x86, make will ask for your password in order to run an offline script lasting several minutes in order to determine the impact of frequency transition on power consumption. Please do not run anything while running this phase.

Note: On Xeon Phi, you will have to run the offline phase manually. Please read [README_MIC](https://github.com/LittleWhite-tb/forest-dvfs/blob/master/README_MIC) for more details about the procedure.

# Usage 
```
    ./FoREST [performance|energy]
```

# Licence
GPLv3

See [COPYRIGHT](https://github.com/LittleWhite-tb/forest-dvfs/blob/master/COPYRIGHT)

