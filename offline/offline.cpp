/*
 * FoREST - Reactive DVFS Control for Multicore Processors
 * Copyright (C) 2013 Universite de Versailles
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <vector>
#include <set>
#include <unistd.h>
#include <stdint.h>

#include "utils.h"

static std::string lpp_path("./lPowerProbe/");
static std::ostringstream libraries;

struct RdtscRatioData {
   double ratio;
   uint64_t rdtsc_value;
   uint64_t time; 
};

struct Data {
   unsigned int cpuId;
   std::vector <unsigned int> freqs;
   std::set <unsigned int> cores;
   std::vector <unsigned int> threads;
   std::vector <unsigned long int> nbIters;
   std::vector <std::vector <BenchResult>*> res; 
#ifdef ARCH_MIC
   std::vector <RdtscRatioData> rdtscData;
#endif
};


/**
 * checkArguments (argc, argv)
 *
 * Checks that the given arguments match the ones required by the program
 */
static inline unsigned int checkArguments (int argc, char *argv []) {
   // Check appropriate number of arguments
   if (argc != 3) {
      std::cerr << "Use: " << argv [0] << " {cpuid} {mic|x86}" << std::endl;
      exit (EXIT_FAILURE);
   }

   // Set appropriate libraries
   libraries.clear ();
   if (!strncmp (argv [2], "mic", 3)) {
      libraries << lpp_path << "/probes/mic_energy/mic_energy.so;" << lpp_path << "/probes/wallclock/wallclock.so;" << lpp_path << "/probes/timer/timer.so";
   } else {
      libraries << lpp_path << "/probes/energy_snb_msr/energy_msr_snb.so;" << lpp_path << "/probes/wallclock/wallclock.so;" << lpp_path << "/probes/timer/timer.so";
   }

   unsigned int cpuId = strtol (argv[1], NULL, 10);

   return cpuId;
}

/**
 * checkEnvironment ()
 *
 * Checks whether the environment can handle the run of the offline phase
 * - Checks the existence and the rights to read the msr files
 * - Check that lPowerProbe is compiled
 */
static inline void checkEnvironment (void) {
   std::ifstream ifs;

   ifs.open ("/dev/cpu/0/msr");
   if (!ifs.good ()) {
      ifs.open ("/dev/msr0");
      if (!ifs.good ()) {
         std::cerr << "Cannot open msr on your machine. There are a few possible reasons for that:"
            << std::endl
            << "- Is the msr module installed and loaded?" << std::endl
            << "- Do you have sufficient rights to read it the msr files?" << std::endl;
         exit (EXIT_FAILURE);
      }
   }
   ifs.close ();

   std::ostringstream oss;
   oss << lpp_path << "lPowerProbe";
   ifs.open (oss.str ().c_str (), std::ios::in | std::ios::binary);

   if (!ifs.good ()) {
      std::cerr << "The " << oss.str () << " project must be compiled." << std::endl;
      exit (EXIT_FAILURE);
   }
}

static inline void printNicePrompt (void) {
   std::cout << "!" << std::endl
             << "!" << std::endl
             << "! In order to measure accurate values, please" << std::endl
             << "!        CLOSE ANY OTHER RUNNING PROGRAM" << std::endl
             << "!" << std::endl
             << "!" << std::endl
             << "Press Enter when you are ready...";
   fgetc (stdin);
}

/**
 * vectorFromFile (vector, filename)
 *
 * Stores in vector the content of a space-splitted file
 */
template <class T>
static inline void vectorFromFile (std::vector <T>& vector,
                                   const char *filename) {
   std::ifstream ifs (filename);

   if (!ifs.good ()) {
      std::cerr << "Error, cannot open file \"" << filename << "\"." << std::endl;
      exit (EXIT_FAILURE);
   }

   T tmp;
   while (ifs >> tmp) {
      vector.push_back (tmp);
   }
}

/**
 * getFreqs (v, cpuId)
 *
 * Stores in v all the frequencies that the thread cpuId can achieve
 */
static inline void getFreqs (Data& data) {
   std::ostringstream oss;
   oss << "/sys/devices/system/cpu/cpu" << data.cpuId << "/cpufreq/scaling_available_frequencies";
   vectorFromFile <unsigned int> (data.freqs, oss.str ().c_str ());
   std::sort (data.freqs.begin (), data.freqs.end ());
}

/**
 * getTopology (threads, cores, cpuId)
 *
 * Stores a list of core ids on the same frequency domain as the cpu defined by cpuid.
 * One id is given per core (ignoring Hyperthreading or similar).
 */
static inline void getTopology (Data& data) {
   std::ostringstream oss;
   std::ifstream ifs;   

   // Get related cores
   oss << "/sys/devices/system/cpu/cpu" << data.cpuId << "/cpufreq/related_cpus";
   vectorFromFile <unsigned int> (data.threads, oss.str ().c_str ());

   // Compute physical cores
   data.cores.clear ();

   // For each core in the related_cpus file, get the first coreId in the
   // according thread_siblings_list file, so that we end up with only one core
   // per physical core
   for (std::vector <unsigned int>::iterator it = data.threads.begin ();
        it != data.threads.end ();
        it++) {
      std::ostringstream oss;
      std::vector <unsigned int> threadSiblings;
      oss << "/sys/devices/system/cpu/cpu" << *it << "/topology/thread_siblings_list";
      //std::cerr << oss.str () << std::endl;
      
      ifs.open (oss.str ().c_str ());
      if (!ifs.good ()) {
         std::cerr << "Error: Cannot open thread siblings list for core " << *it
            << " (file " << oss.str () << std::endl;
         exit (EXIT_FAILURE);
      }
      
      char buf [256];
      ifs.getline (buf, sizeof (buf));
      std::vector <std::string> elems;
      split (buf, ',', elems);
      assert (elems.size () > 0);
      ifs.close ();      
      
      //std::cerr << "elems" << std::endl;
      //printVector <std::string> (elems);

      std::vector <unsigned int> myList;
      for (std::vector <std::string>::iterator it = elems.begin ();
           it != elems.end ();
           it++) {
         std::vector <std::string> threadListStr;
         split ((*it).c_str (), '-', threadListStr);
         assert (threadListStr.size () > 0);

         std::vector <unsigned int> threadListUint;
         vectorToUnsigned (threadListStr, threadListUint);
         //std::cerr << "siblings \'" << (*it).c_str () << "\'" << std::endl;
         //printVector <unsigned int> (threadListUint);
         
         assert (threadListUint.size () > 0);
         if (threadListUint.size () > 1) {
            assert (threadListUint.size () == 2);
            unsigned int min = threadListUint [0];
            unsigned int max = threadListUint [1];
            for (unsigned int i = min; i <= max; i++) {
               myList.push_back (i);
            }
         } else {
            myList.push_back (threadListUint [0]);
         }
         //std::cerr << "myList" << std::endl;
         //printVector <unsigned int> (myList);
      }
      data.cores.insert (myList [0]);
   }
}

/**
 * runBench (nbRepets, cores, coresNb, values)
 * 
 * Stores the power and execution time for the benchmark in values
 * given the number of cores given by the vector cores and coresNb
 * for a total of nbRepets repetitions
 */
static inline void runBench (unsigned long int nbRepets, Data& data,
                             unsigned int coresNb, BenchResult& values,
                             int freqId) {
   std::ostringstream taskMask, cmd;
   unsigned int i = 0;

   // Building the taskMask (for pinning the process on the corresponding cores)
   for (std::set<unsigned int>::iterator it = data.cores.begin ();
        i < coresNb && it != data.cores.end ();
        it++) {
      taskMask << *it;
      if (i+1 < coresNb) {
         taskMask << ";";
      }
      i++;
   }

   cmd << lpp_path << "lPowerProbe -r 5 -d " << coresNb << " -p \"" << taskMask.str ()
       << "\" -o /tmp/results.csv -l \"" << libraries.str () << "\" ./add " << nbRepets; 
   //std::cerr << cmd.str () << std::endl << std::endl;

   // Allow the user to cancel the the offline phase with Ctrl+C if needed
   usleep (10);
   
   FILE *stream = popen (cmd.str ().c_str (), "r");
   if (stream == NULL) {
      std::cerr << "Error: Could not execute command " << cmd.str () << std::endl;
      exit (EXIT_FAILURE);
   }

   char buf [256];
   while (fgets (buf, sizeof (buf), stream));
   pclose (stream);

   std::ifstream ifs ("/tmp/results.csv");
   
   if (!ifs.good ()) {
      std::cerr << "Error: Cannot open results file: /tmp/results.csv" << std::endl;
      exit (EXIT_FAILURE);
   }

   i = 0;
   std::vector <BenchResult> vres;
   while (ifs.getline (buf, sizeof (buf))) {
      std::vector <std::string> elems;
      split (buf, ';', elems); 
      assert (elems.size () == 3);
      std::vector <double> dElems;
      vectorToDouble (elems, dElems);
      BenchResult bres;
      bres.ratio = dElems [0] / dElems [1] * 1e6;
      bres.time = dElems [1] / 1e3;
      bres.rdtsc = dElems [2];

      vres.push_back (bres);
      i++;
   }
   ifs.close ();

   std::sort (vres.begin (), vres.end (), compareBenchResult);

   size_t half = vres.size () / 2;
   values.ratio = vres [half].ratio;
   values.time = vres [half].time;
}

/**
 * getIdealNIters (ms, cores)
 *
 * Estimates the number of iterations to use with the benchmark to reach
 * the "ms" provided execution time, running on a single core
 */
static inline unsigned long int getIdealNIters (unsigned int freqId,
                                                unsigned int ms,
                                   Data& data) {
   unsigned long int nr = 1;
   unsigned long int exectime = 0;
   BenchResult values;

   while (exectime < ms || exectime > 1.5 * ms) {
      runBench (nr, data, 1, values, freqId);

      assert (nr != 0);
      if (values.ratio == 0 || values.time == 0) {
         nr *= 10;      
         continue;
      }

      exectime = values.time;

      if (exectime < ms) {
         // Small values are not relevant
         if (exectime < .1*ms) {
            nr *= 10;
         } else {
            // Assume linear impact of nr on exec time
            // target a little bit above t (1.1*t)
            nr *= ms / exectime;
            nr *= 1.1;
         }
      } else if (exectime > 1.5 * ms) {
         // assume linear impact of nr on exec time
         nr /= 2.;
      }
   }

   return nr;
}

static inline void getIdealIters (Data& data) {
   std::cout << "Determining optimal lPowerProbe configuration";
   fflush (stdout);
   if (hasTB ()) {
      // Target 1 second for all frequencies but TB
      unsigned int i = 0;
      for  (std::vector <unsigned int>::iterator it = data.freqs.begin ();
            it != data.freqs.end ()-1;
            it++) {
         setFreq (data.threads, *it);
         data.nbIters.push_back (getIdealNIters (i++, 1000, data));
         std::cout << ".";
         fflush (stdout);
      }
      setFreq (data.threads, data.freqs.back ());
      data.nbIters.push_back (getIdealNIters (data.freqs.size () - 1, 30000, data));
      std::cout << ".";
   } else {
      // Target 1 second for all frequencies
      unsigned int i = 0;
      for  (std::vector <unsigned int>::iterator it = data.freqs.begin ();
            it != data.freqs.end ();
            it++) {
         setFreq (data.threads, *it);
         data.nbIters.push_back (getIdealNIters (i++, 1000, data));
         std::cout << ".";
         fflush (stdout);
      }
   }

   std::cout << " done";

   // Printing nbIters as we did in Python
   std::cout << "[";
   for (std::vector<unsigned long int>::iterator it = data.nbIters.begin ();
        it != data.nbIters.end ();
        it++) {
      std::cout << (*it) << ",";
   }
   std::cout << "]" << std::endl;
}

/**
 * doProfiling (nbIters, freqs, cores, threads, res)
 *
 * Profiles the different frequencies with all possible cores usage configuration
 * from 1 used core to n (cores.size ())
 */
static inline void doProfiling (Data& data) {
  std::vector <unsigned int>::iterator it; 
  std::vector <unsigned long int>::iterator jt;
   
   for (it = data.freqs.begin (), jt = data.nbIters.begin ();
        it != data.freqs.end (),  jt != data.nbIters.end ();
        it++,
        jt++) {
      setFreq (data.threads, *it);

      std::vector <BenchResult> *coreVector = new std::vector <BenchResult> ();
      coreVector->reserve (data.cores.size ());
      for (unsigned int k = 1; k <= data.cores.size (); k++) {
         BenchResult currentRes;
         runBench (*jt, data, k, currentRes, -1);
         coreVector->push_back (currentRes);
         std::cout << ".";
         fflush (stdout);
      }
      data.res.push_back (coreVector);
   }
   std::cout << "done" << std::endl;
}

static inline void freeResult (std::vector <std::vector <BenchResult>*>& res) {
   for (std::vector <std::vector <BenchResult>*>::iterator it = res.begin ();
        it != res.end ();
        it++) {
      delete *it;
   }
}

/**
 * computeResults (cpuId, freqs, cores, res)
 *
 * Rather stores the results computed in doProfiling in a formatted file
 * that FoREST needs to compute Power Ratios
 */
static inline void computeResults (Data& data) {
   std::ofstream ofs;
   std::ostringstream oss;
   oss << "power_" << data.cpuId << ".cfg";
   ofs.open (oss.str ().c_str ());
   if (!ofs.good ()) {
      std::ostringstream oss;
      oss << "/tmp/power_" << data.cpuId << ".cfg";
      ofs.open (oss.str ().c_str ());
      if (!ofs.good ()) {
         std::cerr << "Error: Could not open " << oss.str () << std::endl;
         exit (EXIT_FAILURE);
      }
      std::cerr << "Warning: Could not create power configuration file in current folder, so it will be saved in " << oss.str () << std::endl;
   }
   
   for (std::vector <unsigned int>::iterator it = data.freqs.begin ();
        it != data.freqs.end ();
        it++) {
      ofs << *it << " ";
   }
   ofs << std::endl;

   for (unsigned int i = 0; i < data.cores.size (); i++) {
      for (unsigned int j = 0; j < data.freqs.size (); j++) {
         ofs << (*(data.res [j]))[i].ratio << " ";
      }
      ofs << std::endl;
   }

#ifdef ARCH_MIC
   for (std::vector<RdtscRatioData>::iterator it = data.rdtscData.begin ();
        it != data.rdtscData.end ();
        it++) {
      ofs << (*it).ratio << " ";
   }
   ofs << std::endl;
#endif

   ofs.close ();
}

#ifdef ARCH_MIC
static inline void getRdtscData (Data &data) {
   unsigned int nr = 10000000;
   BenchResult res;
   for (unsigned int i = 0; i < data.freqs.size (); i++) {
      setFreq (data.threads, data.freqs [i]);
      RdtscRatioData dat = {0,0,0};
      data.rdtscData.push_back (dat);
      for (unsigned int j = 0; j < 15; j++) {
         runBench (nr, data, 1, res, i);
         data.rdtscData [i].rdtsc_value += res.rdtsc;
         data.rdtscData [i].time += res.time * 1e3;
      }
      data.rdtscData [i].rdtsc_value /= 15;
      data.rdtscData [i].time /= 15;
   }
}


static inline void computeRdtscRatios (Data& data) {
   std::vector<RdtscRatioData>::iterator it;
   
   RdtscRatioData max = data.rdtscData.back ();
   for (it = data.rdtscData.begin ();
        it != data.rdtscData.end ();
        it++) {
     (*it).ratio = ((((double)(*it).time / max.time) * max.rdtsc_value) / (*it).rdtsc_value);
   }
}
#endif

int main (int argc, char *argv []) {
   /*
    * Offline phase algorithm
    *
    * 1. Check arguments appropriately
    * 2. Ask the user to be nice with us (and do it nicely)
    * 3. Get the frequencies of given topology node and sort them appropriately
    * 4. Get list of frequency-related cores
    * 5. Handle the special case: only 1 frequency available: in this case,
    * FoREST is useless
    * 6. Get the number of iterations for running the benchmark 30 seconds/freq
    * 7. Compute power ratios for all frequencies with the maximum freq
    * 8. Generate configuration file
    * 9. Yey, we're done
    */
   Data data;

   data.cpuId = checkArguments (argc, argv);
   checkEnvironment ();
   getFreqs (data);
   assert (data.freqs.size () > 0);
   if (data.freqs.size () == 1) {
      std::cerr << "FoREST cannot be beneficial for your system because there is only one frequency to choose from." << std::endl;
      exit (EXIT_FAILURE);
   }
   getTopology (data);
   printNicePrompt ();

   setStdDVFSPolicy (data.threads, "userspace");
   getIdealIters (data);
#ifdef ARCH_MIC
   getRdtscData (data);
   computeRdtscRatios (data);
#endif
   doProfiling (data);
   setStdDVFSPolicy (data.threads, "ondemand");

   computeResults (data);
   freeResult (data.res);

   return EXIT_SUCCESS;
}
