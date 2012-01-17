/*
   Copyright (C) 2011 Exascale Research Center

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
  @file LibProf.h
  @brief The LibProf class header is in this file
 */
#ifndef H_LIBPROF
#define H_LIBPROF

// Define rdtscll
#ifdef __x86_64__
	#define rdtscll(val) do { \
			unsigned int __a,__d; \
			asm volatile("rdtsc" : "=a" (__a), "=d" (__d)); \
			(val) = ((unsigned long)__a) | (((unsigned long)__d)<<32); \
	} while(0)
#elif defined(__i386__)
	#define rdtscll(val) do { \
		asm volatile ("rdtsc" : "=A"(val)); \
		} while(0)
#else
    #error "Cannot implement rtdscll"
#endif


/**
 * @class LibProf
 * @brief The LibProf of a Profiler
 */
class LibProf
{

   public:
      /**
       * @brief Constructor
       */
      LibProf (void);

      /**
       * @brief Destructor
       */
      virtual ~LibProf (void);

      /**
       * @brief Starts watching the counters.
       */
      virtual void start_counters() = 0;

      /**
       * @brief Stop watching the counters.
       */
      virtual void stop_counters() = 0;

      /**
       * @brief Tells the profiler to watch counter for the given thread.
       * @param tid The TID of the concerned thread.
       */
      virtual void attach_to(unsigned long int tid) = 0;

      /**
       * @brief Reads the counter values and reset them.
       * @param values Where to write the couter values.
       */
      virtual void read(long long *values) = 0;

      /**
       * @brief Returns the clock's tick
       * @return The processor tick counter content
       */
      static unsigned long long getTicks ();

      /**
       * @brief Returns the TID of the calling thread.
       * @return The current thread ID.
       */
      static unsigned long int getTID ();
};

#endif
