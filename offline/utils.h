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

#ifndef H_OFFLINEUTILS
#define H_OFFLINEUTILS

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <stdint.h>

struct BenchResult {
   double ratio;
   double time;
   uint64_t rdtsc;
};

std::vector<std::string> &split(const char *s, char delim, std::vector<std::string> &elems) {
   std::string str (s); 
   std::stringstream ss(str);
   std::string item;
   while (std::getline(ss, item, delim)) {
      elems.push_back(item);
   }
  
   return elems;
}

void vectorToUnsigned (std::vector <std::string>& str, std::vector <unsigned int>& uint) {
   uint.clear ();
   for (std::vector <std::string>::iterator it = str.begin ();
        it != str.end ();
        it++) {
      char *end;
      const char *strIt = (*it).c_str ();
      unsigned int convert = strtol (strIt, &end, 10);
      assert (strIt != end);
      uint.push_back (convert);
   }
}

void vectorToDouble (std::vector <std::string>& str, std::vector <double>& db) {
   db.clear ();
   for (std::vector <std::string>::iterator it = str.begin ();
        it != str.end ();
        it++) {
      char *end;
      const char *strIt = (*it).c_str ();
      double convert = strtod (strIt, &end);
      assert (strIt != end);
      db.push_back (convert);
   }
}

template <typename T>
static inline void printVector (std::vector<T>& v) {
   std::cerr << "Vector print:" << std::endl;
   for (typename std::vector<T>::iterator it = v.begin ();
        it != v.end ();
        it++) {
      std::cerr << *it << " ";
   }
   std::cerr << std::endl;
}

template <typename T>
static inline void printSet (std::set<T>& v) {
   std::cerr << "Set print:" << std::endl;
   for (typename std::set<T>::iterator it = v.begin ();
        it != v.end ();
        it++) {
      std::cerr << *it << " ";
   }
   std::cerr << std::endl;
}
   
static inline bool hasTB () {
   std::ifstream ifs;

   ifs.open ("/proc/cpuinfo");
   if (!ifs.good ()) {
      std::cerr << "Error: Cannot find /proc/cpuinfo" << std::endl;
      exit (EXIT_FAILURE);
   }
   
   char buf [2048];
   while (ifs.getline (buf, sizeof (buf))) {
      if (!strncmp (buf, "flags", 5)) {
         if (!strstr (buf, "ida")) {
            ifs.close ();
            return false;
         } else {
            ifs.close ();
            return true;
         }
      }
   }

   return false;
}

void setStdDVFSPolicy (std::vector <unsigned int>& threads,
                       const char *name) {
   std::ofstream ofs;
   std::ostringstream oss;
   for (std::vector<unsigned int>::iterator it = threads.begin ();
        it != threads.end ();
        it++) {
      std::ostringstream oss;
      oss << "/sys/devices/system/cpu/cpu" << *it << "/cpufreq/scaling_governor";
      ofs.open (oss.str ().c_str ());
      if (!ofs.good ()) {
         std::cerr << "Error: Could not open " << oss.str () << std::endl;
         exit (EXIT_FAILURE);
      }

      ofs << name;
      ofs.close ();
   } 
}

void setFreq (std::vector <unsigned int>& threads,
              unsigned long int freq) {
   std::ofstream ofs;
   std::ostringstream oss;
   for (std::vector<unsigned int>::iterator it = threads.begin ();
        it != threads.end ();
        it++) {
      std::ostringstream oss;
      oss << "/sys/devices/system/cpu/cpu" << *it << "/cpufreq/scaling_setspeed";
      ofs.open (oss.str ().c_str ());
      if (!ofs.good ()) {
         std::cerr << "Error: Could not open " << oss.str () << std::endl;
         exit (EXIT_FAILURE);
      }

      ofs << freq;
      ofs.close ();
   } 
}

bool compareBenchResult (const BenchResult& a, const BenchResult& b) {
   if (a.time == b.time) {
      return a.ratio < b.ratio;
   }
   return a.time < b.time;
}

#endif
