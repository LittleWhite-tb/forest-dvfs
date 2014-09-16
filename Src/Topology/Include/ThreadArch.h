/**
 * @file ThreadArch.h
 *
 * Includes the Thread-derived class header corresponding to the
 * target architecture.
 */

#ifdef ARCH_MIC
   #include "ThreadMic.h"
   #ifndef THREADCLASS
      #define THREADCLASS ThreadMic
   #endif
#endif
#ifdef ARCH_X86
   #include "ThreadX86.h"
   #ifndef THREADCLASS
      #define THREADCLASS ThreadX86
   #endif
#endif

