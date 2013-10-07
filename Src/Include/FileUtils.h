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

#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "glog/logging.h"

/**
 * Useful functions for reading files in sysfs considering their
 * format specificities
 */
class FileUtils
{
public:
   /**
    * Try to open a file considering several possible locations
    * written in the filenames vector, ordered by location preference.
    *
    * The opened stream (if any amongst all the locations) is stored in
    * fs, and flags refers to standard fstream flags for file opening)
    */
   static void tryToOpen (const std::vector<std::string>& filenames,
                                  std::fstream& fs,
                                  const std::ios_base::openmode& flags) {
      std::vector<std::string>::const_iterator it;

      for (it = filenames.begin (); it != filenames.end (); it++) {
         fs.open ((*it).c_str (), flags);
   
         if (fs.is_open ()) {
            return; // We found it, so we're all set
         }
      }
   
      std::ostringstream err;
      err << "Error: Could not open any of the following files: ";
      for (it = filenames.begin (); it != filenames.end (); it++) {
         err << (*it) << " ";
      }
      LOG (FATAL) << err.str () << std::endl;
   }

   /**
    * Read a line in the ifs file stream and store its space-separated
    * values in the T-typed v vector.
    */
   template <typename T>
   static bool readLine (std::vector<T>& v, std::fstream& ifs) {
      T val;
      std::string line;
      std::getline (ifs, line, '\n');
      std::istringstream iss (line);

      iss >> std::ws;
      while (iss >> val >> std::ws) {
         v.push_back (val);
      }

      return ifs;
   }
};

#endif
