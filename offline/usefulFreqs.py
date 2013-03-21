#!/usr/bin/env python
# -*- encoding: utf8 -*-

#
# FoREST - Reactive DVFS Control for Multicore Processors
# Copyright (C) 2013 Universite de Versailles
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import subprocess as sp, multiprocessing as mp, sys
import os

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

LPP_PATH="./lPowerProbe/"
RESULT_FILE="/tmp/output.csv"

#----------------------------------

def runBench (nr, cores):
    '''
    Returns the power and execution time for the benchmark.
    Returns None in case of measurement error. Otherwise result is a couple
    (time_in_ms, cpu_power_in_W)

    nr is the number of iterations to use
    cores is the id of the cores on whitch the test must be replicated
    '''

    taskMask = ";".join([str(c) for c in cores])

    cmd = LPP_PATH + 'lPowerProbe -r 5 -d ' + str(len(cores)) + ' -p "' + taskMask + '" -o ' + RESULT_FILE + ' ./add ' + str(nr)
    # print cmd
    ex = sp.Popen(cmd, shell=True, stderr=sp.STDOUT, stdout=sp.PIPE)
    ex.communicate()[0]

    if ex.returncode != 0:
      sys.exit (0)

    try:
      fd = open(RESULT_FILE)
    except IOError as e:
      sys.stderr.write ("Error: Cannot launch lPowerProbe.\n- Is it cloned at the base of your nfs directory ?\n")
      sys.stderr.write ("- Are you connected to the local network ?\n- Did you modprobe msr and chmod it properly ?\n- Did you chmod cpufreq folder correctly ?\n- Did you echo -1 in the event_paranoid file ?\n- Are you running a SandyBridge or more recent architecture ?\n")
      sys.exit (0)

    data = []
    for ln in fd.readlines()[1:]: 
      ln = ln.split(";")

      # error in data file
      if len(ln) > 7:
          return None

      # Data format: {ms, Watts}, ml output: {J, us}
      data.append((float(ln[1]) / 1e3, float(ln[0]) / float (ln [1]) * 1e6))
    fd.close()
    data.sort()
    
    os.remove(RESULT_FILE)

    return data[len(data) / 2]

#----------------------------------

def getIdealNIters (t, cores):
   '''
   Estimates the number of iterations to use with the benchmark to reach the provided execution time.

   t is the target execution time for the benchmark (in ms)
   '''

   nr = 1
   exectime = 0
   while exectime < t or exectime > 1.5 * t:   
      r = runBench(nr, cores[:1])

      if r is None:
         nr = nr * 10
         continue

      exectime = r[0]

      if exectime < t:
         # small values are not relevant
         if exectime < 0.1 * t:
            nr = nr * 10
         else:
            # assume linear impact of nr on exec time
            # target a little bit above t (1.1 * t)
            nr = nr * (t / exectime)
            nr = int(nr * 1.1)
      elif exectime > 1.5 * t:
         # assume linear impact of nr on exec time
         nr = int(nr / 2)

   return nr

#----------------------------------

def getRelatedCores (cpuid):
   '''Returns the number of cores on the system'''

   fd = open ("/sys/devices/system/cpu/cpu" + str (cpuid) + "/cpufreq/related_cpus")
   data = fd.read ()
   fd.close ()

   data = data.split ()

   return [int (f) for f in data];

#----------------------------------

def getPhysicalCores (cpuid):
   '''Returns a list of core ids on the same frequency domain as the cpu defined by cpuid.
   One id is given per core (ignoring multithread).
   '''

   # Retrieve the list of related cores to the given cpuid
   relatedCores = getRelatedCores (cpuid)

   physicalCores = []
   # only keep one id per core
   for c in relatedCores:
      fd = open ("/sys/devices/system/cpu/cpu" + str (c) + "/topology/thread_siblings_list")
      data = fd.read ()
      fd.close()
      data = data.split (",")
      physicalCores.append(int(data[0]))

   physicalCores = list(set(physicalCores))
   physicalCores.sort()

   return physicalCores

#----------------------------------

def getFreqs (cpuid):
    """Returns the sorted list of all the available frequencies."""

    fd = open("/sys/devices/system/cpu/cpu" + str (cpuid) + "/cpufreq/scaling_available_frequencies")
    data = fd.read()
    fd.close()

    data = data.split()

    return [int(f) for f in data]

#----------------------------------

def setFreq (f):
   '''Sets the given frequency'''

   try:
      for c in cores:
         fd = open("/sys/devices/system/cpu/cpu" + str(c) + "/cpufreq/scaling_governor", "w")
         fd.write("userspace")
         fd.flush()
         fd.close()

         fd = open("/sys/devices/system/cpu/cpu" + str(c) + "/cpufreq/scaling_setspeed", "w")
         fd.write(str(f))
         fd.flush()
         fd.close()
   except IOError as e:
      sys.stderr.write ("Error: Cannot open frequency setter file: " + e.strerror + "\n")

#----------------------------------

def setOnDemand ():
    '''Sets the dvfs governor to 'ondemand'.'''

    for c in cores:
         fd = open("/sys/devices/system/cpu/cpu" + str(c) + "/cpufreq/scaling_governor", "w")
         fd.write("ondemand")
         fd.flush()
         fd.close()

#----------------------------------

def hasTB():
   '''Returns true if the current computer supports TurboBoost.'''

   fd = open("/proc/cpuinfo")
   for ln in fd:
      if ln.lower().startswith("flags"):
         if " ida " in ln:
            fd.close()
            return True
         else:
            fd.close()
            return False

   # flags not found
   fd.close()

   return False

#----------------------------------
# Main

if len (sys.argv) != 2:
   print sys.argv[0] + " {cpuid}"
   sys.exit (0)

if not os.path.exists("/dev/cpu/0/msr"):
   print "MSR is not available on your machine. Please install it and start it with modprobe"
   sys.exit (0)
   
if not os.path.exists("./add"):
   print "Test program './add' not compiled - aborting"
   sys.exit (0)

if not os.path.exists(LPP_PATH + "lPowerProbe"):
   print "lPowerProbe program " + LPP_PATH + " not compiled - aborting"
   sys.exit (0)

# ask the user to be nice with us
print "!"
print "!"
print "! In order to measure accurate values, please"
print "!      CLOSE ANY OTHER RUNNING PROGRAM"
print "!"
print "!"
raw_input("press Enter when you are ready...")

# Get list of frequencies and sort'em
freqs = getFreqs (int (sys.argv [1]))
freqs.sort ()

# Get list of related cores (by frequency)
cores = getPhysicalCores (sys.argv [1])

# Name of the output configuration file
configFile = "power_" + sys.argv [1] + ".cfg"

# special case of 1 available frequency
if len(freqs) == 1:
   sys.stderr.write ("Info: Only 1 frequency is available. Using FoREST is useless because you cannot take advantage of DVFS techniques. A configuration file has been created anyway.");
   fd = 0
   try:
      fd = open (configFile, 'w')
   except IOError:
      print "WARNING: Config file written in /tmp"
      fd = open ("/tmp/" + configFile, 'w') #At this point, I can't do more, it's failing hard
   fd.write (str (freqs [0]) + "\n")
   for i in cores:
      fd.write ("0\n")
   fd.close ()
   sys.exit(0)

sys.stdout.write("Determining optimal lPowerProbe configuration")
sys.stdout.flush()
if hasTB():
   # target 1 second for all frequencies but TB
   nr = []
   for f in freqs[:-1]:
      setFreq(f)
      nr.append(getIdealNIters(1000, cores))
      sys.stdout.write(".")
      sys.stdout.flush()

   # target 30 seconds for TurboBoost
   setFreq(freqs[-1])
   nr.append(getIdealNIters(30000, cores))
   sys.stdout.write(".")
   sys.stdout.flush()
else:
   # target 1 second for all frequencies
   nr = []
   for f in freqs:
      setFreq(f)
      nr.append(getIdealNIters(1000, cores))
      sys.stdout.write(".")
      sys.stdout.flush()

sys.stdout.write(" done " + str(nr) + "\n")

sys.stdout.write("Profiling")
sys.stdout.flush()

# get time and power for all frequencies and number of cores
res = []
for f, nbrep in zip(freqs, nr):
   setFreq(f)

   res.append([])
   id = 0
   for c in range(len(cores)):
      res[-1].append(runBench(nbrep, cores[:c + 1]))
      sys.stdout.write(".")
      sys.stdout.flush()
sys.stdout.write(" done\n")

# Reset policy to default ondemand
setOnDemand ()

# Initialize the list of frequencies to avoid for energy
freqsToDelete = [False] * len(freqs)

# compute the power ratio for all frequencies with the maximum freq
ratios = []
sys.stdout.write("Computing results... ")
sys.stdout.flush()
for i in range(len(freqs)):
   for l in range(len(freqs)):
      if i == l:
         continue

      deleteFreq = True

      if freqs [i] > freqs [l]:
         for k in cores:
            if res [l][k][1] / res [i][k][1] < 1.05:
               deleteFreq = False
               break

         if deleteFreq:
            freqsToDelete [l] = True
      else:
         for k in cores:
            # for turboboost, use actual execution times
            if hasTB() and i == len(freqs) - 1 or l == len(freqs) - 1:
               if (res[l][k][0] * res[l][k][1]) / (res[i][k][0] * res[i][k][1]) < 1.05:
                  deleteFreq = False
                  break
            else:
               if (res [l][k][1] / res [i][k][1]) * (float(freqs [i]) / float(freqs [l])) < 1.05:
                  deleteFreq = False
                  break

         if deleteFreq:
            freqsToDelete [l] = True

sys.stdout.write("done\n")

# Energy configuration file generation
try:
   fdPerf = open (configFile, 'w')
except IOError:
   print "WARNING: Config file written in /tmp"
   fdPerf = open ("/tmp/" + configFile, 'w') #At this point, I can't do more, it's failing hard

highestFreq = 0
for i in range(len(freqs)):
   fdPerf.write (str (freqs [i]) + " ")
fdPerf.write ("\n")

for i in cores:
   for j in range(len(freqs)):
      fdPerf.write (str (res [j][i][1]) + " ")
   fdPerf.write ("\n")
fdPerf.flush()
fdPerf.close ()

