#!/usr/bin/env python
# -*- encoding: utf8 -*-

import os, subprocess as sp, multiprocessing as mp, sys

"""
The power consumption of a CPU is C * f * V² = W
C depends on the program
considering the consumption of a program at two frequencies f1 and f2 (W1 and W2), W1/W2 = f1*V1²/f2*V2² (<- program independant!!!)
The execution time of the program under the two frequencies is t1 and t2
t1/t2 * W1/W2 = e1/e2 (e1 and e2 are energy consumption for both frequencies)
Note the minimal value for t1/t2 is (1/f1)/(1/f2)
if the program as the lowest possible value for t1/t2 (i.e. it is purely CPU bound), then the ratio e1/e2 is minimal
and all programs have higher ratios
now if e1/e2 for this program is > 1, then all programs have e1/e2 > 1, meaning that they will never save energy when going into f1 compared to f2

This concept is exploited here to evaluate the usefulness of all frequencies. A CPU bound benchmark is used to estimate the minimal t1/t2 as
some frequencies such as TurboBoost lie on the frequency declared to the OS. Different number of active cores are tested as this number of active core can
impact the actual frequency applied by TurboBoost.
"""


#----------------------------------

def runBench(nr, nc):
   '''
   Returns the power and execution time for the benchmark.
   Returns None in case of measurement error. Otherwise result is a couple
   (time_in_ms, cpu_power_in_W)

   nr is the number of iterations to use
   '''

   cmd = '$HOME/nfs/microlaunch/microlaunch --kernelname "add.s" --nbprocess ' + str(nc) + ' --repetition ' + str(nr)\
         + ' --metarepetition 5 --basename "add"  --info "raw;raw" --evallib '\
         + '"$HOME/nfs/microlaunch/Libraries/power_snb_msr/libraries/energy_msr_snb.so;$HOME/nfs/microlaunch/Libraries/wallclock/wallclock.so"'\
         + ' --output-dir "/tmp"'

   ex = sp.Popen(cmd, shell=True, stderr=sp.STDOUT, stdout=sp.PIPE)
   ex.communicate() [0]
   
   # fetch result
   fd = open("/tmp/kernel_add_2500000.csv")
   data = []
   for ln in fd.readlines()[1:]:
      ln = ln.split(",")

      # error in data file
      if len(ln) > 7:
         return None

      data.append((float(ln[1]) / 1000, float(ln[0]) / (float(ln[1]) / 1000000)))
   fd.close()

   data.sort()
   return data[len(data) / 2]

#----------------------------------

def getIdealNIters(t):
   '''
   Estimates the number of iterations to use with the benchmark to reach the provided execution time.

   t is the target execution time for the benchmark (in ms)
   '''

   assert (t > 1000)

   nr = 1
   exectime = 0
   while exectime < t or exectime > 2 * t:
      r = runBench(nr, 1)

      if r is None:
         nr = nr * 10
         continue

      exectime = r[0]

      if exectime < t:
         # less than a second is not relevant
         if exectime < 1000:
            nr = nr * 10
         else:
            # assume linear impact of nr on exec time
            # target a little bit above t (1.1 * t)
            nr = nr * (t / exectime)
            nr = int(nr * 1.1)
      elif exectime > 2 * t:
         # assume linear impact of nr on exec time
         nr = int(nr / 2)

   return nr

#----------------------------------

def getNbCores():
   '''Returns the number of cores on the system'''

   return mp.cpu_count()

#----------------------------------

def getFreqs():
   """Returns the sorted list of all the available frequencies."""

   fd = open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies")
   data = fd.read()
   fd.close()

   data = data.split()

   return [int(f) for f in data]

#----------------------------------

def setFreq(f):
   '''Sets the given frequency'''

   for c in range(getNbCores()):
      fd = open("/sys/devices/system/cpu/cpu" + str(c) + "/cpufreq/scaling_governor", "w")
      fd.write("userspace")
      fd.flush()
      fd.close()

      fd = open("/sys/devices/system/cpu/cpu" + str(c) + "/cpufreq/scaling_setspeed", "w")
      fd.write(str(f))
      fd.flush()
      fd.close()

#----------------------------------

def setOnDemand():
   '''Sets the dvfs governor to 'ondemand'.'''

   for c in range(getNbCores()):
      fd = open("/sys/devices/system/cpu/cpu" + str(c) + "/cpufreq/scaling_governor", "w")
      fd.write("ondemand")
      fd.flush()
      fd.close()


#----------------------------------

freqs = getFreqs()

# special case of 1 freq
if len(freqs) == 1:
   print freqs[0]
   sys.exit(0)

# consider max freq to get nr
setFreq(freqs[-1])

# try to reach 30 seconds with the benchmark to let TB react significantly
nr = getIdealNIters(30000)

# get time and power for all frequencies and number of cores
res = []
for f in freqs:
  setFreq(f)

  res.append([])
  for c in range(getNbCores()):
     res[-1].append(runBench(nr, c + 1))

# compute the energy ratio for all frequencies with the minimal freq
er = []
# compute the power ratio for all frequencies with the maximum freq
ratios = []
for nf in range(1, len(freqs)):
  er.append([])
  for nc in range(getNbCores()):
     er[-1].append((res[nf][0] * res[nf][1]) / (res[0][0] * res[0][1]))
  ratios.append (res [nf][1] / res [0][1] * 100 )

# compare minimal freq to the next one
er = [] + er
for nc in range(getNbCores()):
   er[0].append((res[0][0] * res[0][1]) / (res[1][0] * res[1][1]))

# only print frequencies that can be useful
for nf in range(len(freqs)):
   er[nf] = [rf < 1 for rf in er[nf]]
   if any(er[nf]):
      print freqs[nf]


for nf in range (len (freqs)):
   er [nf] = [rf < 1 for rf in er [nf]]
   if any (er [nf]):
      print ratios [nf]

