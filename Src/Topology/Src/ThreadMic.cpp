#include "ThreadMic.h"
#include "DVFSUnit.h"

namespace FoREST {

bool ThreadMic::computeIPC (unsigned int freqId) {
   uint64_t retired = this->retired.getValue (freqId);
   uint64_t time = this->time_ [freqId].current;
   bool hwcPanic = false;

   if (time == 0) {
      LOG (WARNING) << "no time elapsed since last measurement" << std::endl;
      return false;
   }

   // Computes ipc value
   float ipc = retired / (this->unit_.getRdtscRatio (freqId) * time);
   this->ipc_ [freqId] = ipc;

   // Handle irrational ipc values
   if (ipc < 0 || isnan (ipc)) {
      hwcPanic = true;
   }

   return hwcPanic; 
}

} // namespace FoREST
