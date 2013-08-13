#ifndef H_THREADMIC
#define H_THREADMIC

#include "Thread.h"
#include "Counter.h"
#include "Profiler.h"

namespace FoREST {

class ThreadMic : public Thread {
private:
   Counter retired;
   Counter llcMisses;
   Counter unhalted;
   uint64_t lastUsageComputation;
   uint64_t usageElapsedTime;

public:
   ThreadMic (unsigned int id, unsigned int nbFrequencies, Profiler& profiler, uint64_t threshold) :
      Thread (id, nbFrequencies, profiler, threshold),
      retired ("INSTRUCTIONS_EXECUTED", nbFrequencies),
      unhalted ("CPU_CLK_UNHALTED"),
      llcMisses ("L2_DATA_PF2_MISS")
   {
      this->profiler_.open (this->retired, id);
      this->profiler_.open (this->llcMisses, id);
      this->profiler_.open (this->unhalted, id);
   }

   virtual ~ThreadMic () {
      this->profiler_.close (this->retired);
      this->profiler_.close (this->llcMisses);
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
      Profiler::readTsc (this->execTime_);
      return this->profiler_.read (this->llcMisses);
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
         
         //std::cerr << "#" << this->id_ << ": " << std::setw (4) << (int)(res*100) << " ";

         this->usage_ = rest_min (res, 1);

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

      //std::cerr << "IPC #" << this->id_ << ": " << this->ipc_ [freqId] << " ";

      return hwcPanic; 
   }
   inline void computeLLCRatio () {
      uint64_t misses = llcMisses.getValue ();
      this->llcRatio_ = misses;
   }
};

} // namespace FoREST

#endif /* H_THREADMIC */
