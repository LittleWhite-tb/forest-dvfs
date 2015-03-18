README For INTEL XEON PHI

Please follow these instructions when FoREST is compiled and successfully transfered on the Xeon Phi card. For compilation instructions, please read the README file.

As the compilation needs to be done on the host machine, the offline phase must be run manually on the co-processor. Go on the FoREST folder once the project is built, and run the following command:
```
   ./offline_run.sh
```

You are now all set! You can run FoREST:
```
   ./FoREST {energy|performance}
```
