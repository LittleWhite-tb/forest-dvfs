#ifndef H_THREADMIC
#define H_THREADMIC

#include "Thread.h"
#include "Counter.h"
#include "Profiler.h"

namespace FoREST {

class DVFSUnit;

class ThreadMic : public Thread {
private:
   Counter retired; 
   Counter unhalted;
   uint64_t lastUsageComputation;
   uint64_t usageElapsedTime;

public:
   ThreadMic (unsigned int id, unsigned int nbFrequencies, Profiler& profiler, DVFSUnit& unit, uint64_t threshold) :
      Thread (id, nbFrequencies, profiler, unit, threshold),
      retired ("INSTRUCTIONS_EXECUTED", nbFrequencies),
      unhalted ("CPU_CLK_UNHALTED")
   {
      this->profiler_.open (this->retired, id);
      this->profiler_.open (this->unhalted, id);
      this->usage_ = 1;
   }

   virtual ~ThreadMic () {
      this->profiler_.close (this->retired); 
      this->profiler_.close (this->unhalted);
   }

   inline bool reset (unsigned int freqId) {
      Profiler::readTsc (this->time_ [freqId]);
      return this->profiler_.read (this->retired, freqId);
   }

   inline bool read (unsigned int freqId) {
      return reset (freqId);
   }

   inline bool resetExec () {
      return true;
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
      if (this->hasToUpdateUsage ()) {
         this->profiler_.read (this->unhalted);
         uint64_t unhalted = this->unhalted.getValue ();

         float res = unhalted / (1. * this->usageElapsedTime);
         this->usage_ = rest_min (res, 1);
         
         this->lastUsageComputation = rdtsc ();
      }
   }
   
   bool computeIPC (unsigned int freqId);

   inline void computeLLCRatio () {}
};

} // namespace FoREST

#endif /* H_THREADMIC */
