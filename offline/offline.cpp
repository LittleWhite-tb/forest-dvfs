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

#include "utils.h"

static std::string lpp_path("./lPowerProbe/");
static std::ostringstream libraries;

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
      libraries << lpp_path << "/probes/mic_energy/mic_energy.so;" << lpp_path << "/probes/wallclock/wallclock.so";
   } else {
      libraries << lpp_path << "/probes/energy_snb_msr/energy_msr_snb.so;" << lpp_path << "/probes/wallclock/wallclock.so";
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
static inline void getFreqs (std::vector <unsigned int>& v,
                                      unsigned int cpuId) {
   std::ostringstream oss;
   oss << "/sys/devices/system/cpu/cpu" << cpuId << "/cpufreq/scaling_available_frequencies";
   vectorFromFile <unsigned int> (v, oss.str ().c_str ());
   std::sort (v.begin (), v.end ());
}

/**
 * getTopology (threads, cores, cpuId)
 *
 * Stores a list of core ids on the same frequency domain as the cpu defined by cpuid.
 * One id is given per core (ignoring Hyperthreading or similar).
 */
static inline void getTopology (std::vector <unsigned int>& threads,
                                std::set <unsigned int>& cores,
                                     unsigned int cpuId) {
   std::ostringstream oss;
   std::ifstream ifs;   

   // Get related cores
   oss << "/sys/devices/system/cpu/cpu" << cpuId << "/cpufreq/related_cpus";
   vectorFromFile <unsigned int> (threads, oss.str ().c_str ());

   // Compute physical cores
   cores.clear ();

   // For each core in the related_cpus file, get the first coreId in the
   // according thread_siblings_list file, so that we end up with only one core
   // per physical core
   for (std::vector <unsigned int>::iterator it = threads.begin ();
        it != threads.end ();
        it++) {
      std::ostringstream oss;
      std::vector <unsigned int> threadSiblings;
      oss << "/sys/devices/system/cpu/cpu" << *it << "/topology/thread_siblings_list";
      //std::cerr << oss.str () << std::endl;
      
      ifs.open (oss.str ().c_str ());
      if (!ifs.good ()) {
         std::cerr << "Error: Cannot open thread siblings list for core " << *it << " (file " << oss.str ()
                   << std::endl;
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
      cores.insert (myList [0]);
   }
}

/**
 * runBench (nbRepets, cores, coresNb, values)
 * 
 * Stores the power and execution time for the benchmark in values
 * given the number of cores given by the vector cores and coresNb
 * for a total of nbRepets repetitions
 */
static inline void runBench (unsigned long int nbRepets, std::set <unsigned int>& cores,
                             unsigned int coresNb, BenchResult& values) {
   std::ostringstream taskMask, cmd;

   unsigned int i = 0;
   for (std::set<unsigned int>::iterator it = cores.begin ();
        i < coresNb && it != cores.end ();
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
   //int r = system (cmd.str ().c_str ());
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
      assert (elems.size () == 2);
      std::vector <double> dElems;
      vectorToDouble (elems, dElems);
      BenchResult bres;
      bres.ratio = dElems [0] / dElems [1] * 1e6;
      bres.time = dElems [1] / 1e3;

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
static inline unsigned long int getIdealNIters (unsigned int ms,
                                   std::set <unsigned int>& cores) {
   unsigned long int nr = 1;
   unsigned long int exectime = 0;
   BenchResult values;

   while (exectime < ms || exectime > 1.5 * ms) {
      runBench (nr, cores, 1, values);

      //std::cerr << "nr = " << nr << std::endl;
      //std::cerr << "exectime = " << exectime << std::endl;
      assert (nr != 0);
      if (values.ratio == 0 || values.time == 0) {
         nr *= 10;
         //std::cerr << "x10" << std::endl;
         continue;
      }

      //std::cerr << "I was here" << std::endl;

      exectime = values.time;

      //std::cerr << "et = " << exectime << " , ms = " << ms << std::endl;
      if (exectime < ms) {
         // Small values are not relevant
         if (exectime < .1*ms) {
            nr *= 10;
            //std::cerr << "<0.1*t" << std::endl;
         } else {
            // Assume linear impact of nr on exec time
            // target a little bit above t (1.1*t)
            //std::cerr << "*1.1" << std::endl;
            nr *= ms / exectime;
            nr *= 1.1;
         }
      } else if (exectime > 1.5 * ms) {
         // assume linear impact of nr on exec time
         //std::cerr << ">1.5*t" << std::endl;
         nr /= 2.;
      }
   }

   return nr;
}

static inline void getIdealIters (std::vector <unsigned long int>& nbIters,
                                  std::vector <unsigned int>& freqs,
                                  std::set <unsigned int>& cores,
                                  std::vector <unsigned int>& threads) {
   std::cout << "Determining optimal lPowerProbe configuration";
   fflush (stdout);
   if (hasTB ()) {
      // Target 1 second for all frequencies but TB
      for  (std::vector <unsigned int>::iterator it = freqs.begin ();
            it != freqs.end ()-1;
            it++) {
         setFreq (threads, *it);
         nbIters.push_back (getIdealNIters (1000, cores));
         std::cerr << nbIters.back () << std::endl;
         exit (EXIT_SUCCESS);
         std::cout << ".";
         fflush (stdout);
      }
      setFreq (threads, freqs.back ());
      nbIters.push_back (getIdealNIters (30000, cores));
      std::cout << ".";
   } else {
      // Target 1 second for all frequencies
      for  (std::vector <unsigned int>::iterator it = freqs.begin ();
            it != freqs.end ();
            it++) {
         setFreq (threads, *it);
         nbIters.push_back (getIdealNIters (1000, cores));
         std::cout << ".";
         fflush (stdout);
      }
   }

   std::cout << " done";

   // Printing nbIters as we did in Python
   std::cout << "[";
   for (std::vector<unsigned long int>::iterator it = nbIters.begin ();
        it != nbIters.end ();
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
static inline void doProfiling (std::vector <unsigned long int>& nbIters,
                                std::vector <unsigned int>& freqs,
                                std::set <unsigned int>& cores,
                                std::vector <unsigned int>& threads,
                                std::vector <std::vector <BenchResult>*>& res) {
  std::vector <unsigned int>::iterator it; 
  std::vector <unsigned long int>::iterator jt; 
   
   for (it = freqs.begin (), jt = nbIters.begin ();
        it != freqs.end (),  jt != nbIters.end ();
        it++,
        jt++) {
      setFreq (threads, *it);

      std::vector <BenchResult> *coreVector = new std::vector <BenchResult> ();
      coreVector->reserve (cores.size ());
      for (unsigned int k = 1; k <= cores.size (); k++) {
         BenchResult currentRes;
         runBench (*jt, cores, k, currentRes);
         coreVector->push_back (currentRes);
         std::cout << ".";
         fflush (stdout);
      }
      res.push_back (coreVector);
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
static inline void computeResults (unsigned int cpuId,
                                   std::vector <unsigned int>& freqs,
                                   std::set <unsigned int>& cores,
                                   std::vector <std::vector <BenchResult>*>& res) {
   std::ofstream ofs;
   std::ostringstream oss;
   oss << "power_" << cpuId << ".cfg";
   ofs.open (oss.str ().c_str ());
   if (!ofs.good ()) {
      std::ostringstream oss;
      oss << "/tmp/power_" << cpuId << ".cfg";
      ofs.open (oss.str ().c_str ());
      if (!ofs.good ()) {
         std::cerr << "Error: Could not open " << oss.str () << std::endl;
         exit (EXIT_FAILURE);
      }
      std::cerr << "Warning: Could not create power configuration file in current folder, so it will be saved in " << oss.str () << std::endl;
   }
   
   for (std::vector <unsigned int>::iterator it = freqs.begin ();
        it != freqs.end ();
        it++) {
      ofs << *it << " ";
   }
   ofs << std::endl;

   for (unsigned int i = 0; i < cores.size (); i++) {
      for (unsigned int j = 0; j < freqs.size (); j++) {
         ofs << (*(res [j]))[i].ratio << " ";
      }
      ofs << std::endl;
   }

   ofs.close ();
}


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
   std::vector <unsigned int> freqs;
   std::set <unsigned int> cores;
   std::vector <unsigned int> threads;
   std::vector <unsigned long int> nbIters;
   std::vector <std::vector <BenchResult>*> res;
   std::vector <float> ratios;

   unsigned cpuId = checkArguments (argc, argv);
   checkEnvironment ();
   getFreqs (freqs, cpuId);
   assert (freqs.size () > 0);
   if (freqs.size () == 1) {
      std::cerr << "FoREST cannot be beneficial for your system because there is only one frequency to choose from." << std::endl;
      exit (EXIT_FAILURE);
   }
   getTopology (threads, cores, cpuId);
   printNicePrompt ();

   setStdDVFSPolicy (threads, "userspace");
   getIdealIters (nbIters, freqs, cores, threads);
   doProfiling (nbIters, freqs, cores, threads, res);
   setStdDVFSPolicy (threads, "ondemand");

   computeResults (cpuId, freqs, cores, res);
   freeResult (res);

   return EXIT_SUCCESS;
}
