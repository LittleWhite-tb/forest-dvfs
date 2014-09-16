#ifndef H_THREADX86
#define H_THREADX86

#include "Thread.h"
#include "Counter.h"
#include "Profiler.h"

namespace FoREST {

/**
 * Implements the Thread class for x86 architectures
 */
class ThreadX86 : public Thread {
private:
   /**
    * Counter representing the number of cycles executed per unit of time.
    */
   Counter retired;

   /**
    * Counter representing the number of unhalted cycles executed per unit of time.
    */
   Counter unhalted;

   /**
    * Counter representing the number of misses in the last level of cache.
    */
   Counter llcMisses;

   /**
    * Counter representing the number of llc references.
    */
   Counter llcReferences;

   /**
    * Time when the last thread usage computation occured
    */
   uint64_t lastUsageComputation;
   
   /**
    * Time elapsed between now and last usage computation 
    */
   uint64_t usageElapsedTime;

public:
   ThreadX86 (unsigned int id, unsigned int nbFrequencies, Profiler& profiler, DVFSUnit& unit, uint64_t threshold) :
      Thread (id, nbFrequencies, profiler, unit, threshold),
      retired ("INST_RETIRED:ANY_P", nbFrequencies),
      unhalted ("UNHALTED_REFERENCE_CYCLES"),
      llcMisses ("LLC_MISSES"),
      llcReferences ("LLC_REFERENCES"),
      lastUsageComputation (rdtsc ()),
      usageElapsedTime (0)
   {
      this->profiler_.open (this->retired, id);
      this->profiler_.open (this->unhalted, id);
      this->profiler_.open (this->llcMisses, id);
      this->profiler_.open (this->llcReferences, id);
   }

   virtual ~ThreadX86 () {
      this->profiler_.close (this->retired);
      this->profiler_.close (this->unhalted);
      this->profiler_.close (this->llcMisses);
      this->profiler_.close (this->llcReferences);
   }

   inline bool reset (unsigned int freqId) {
      Profiler::readTsc (this->time_ [freqId]);
      return this->profiler_.read (this->retired, freqId);
   }

   inline bool read (unsigned int freqId) {
      return reset (freqId);
   }

   inline bool resetExec () {
      Profiler::readTsc (this->execTime_);
      bool ret = this->profiler_.read (this->llcReferences);
      ret |= this->profiler_.read (this->llcMisses);
      return ret;
   }

   inline bool readExec () {
      return resetExec ();
   }
   
   /**
    * Returns whether the usage computation has to be updated
    * e.g usage computation is too old
    */
   inline bool hasToUpdateUsage () {
      this->usageElapsedTime = rdtsc () - this->lastUsageComputation;
      return this->usageElapsedTime > TIME_THRESHOLD;
   }

   inline void computeUsage () {
      if (this->hasToUpdateUsage ()) { // Only updates the usage if necessary
         this->profiler_.read (this->unhalted);
         uint64_t unhalted = this->unhalted.getValue ();
         
         float res = unhalted / (1. * this->usageElapsedTime);

         //std::cerr << "#" << this->id_ << ": " << std::setw (4) << (int)(res*100) << " ";
         
         // Rationalize the usage if it goes beyond a ratio of 1
         this->usage_ = rest_min (res, 1);

         // Update lastUpdate to new time reference
         this->lastUsageComputation = rdtsc ();
      }
   }

   inline bool computeIPC (unsigned int freqId) {
      uint64_t retired = this->retired.getValue (freqId);
      uint64_t time = this->time_ [freqId].current;
      bool hwcPanic = false;

      if (time == 0) {
         LOG (WARNING) << "no time elapsed since last measurement" << std::endl;
         return false;
      }

      // Computes ipc value
      float ipc = retired / (1. * time);
      this->ipc_ [freqId] = ipc;

      // Handle irrational ipc values
      if (ipc < 0 || isnan (ipc)) {
         hwcPanic = true;
      }

      return hwcPanic;
   }

   inline void computeLLCRatio () {
      uint64_t misses = llcMisses.getValue ();
      uint64_t references =  llcReferences.getValue ();

      this->llcRatio_ = misses / (1. * references);
   }
};

} // namespace FoREST

#endif /* H_THREADMIC */
