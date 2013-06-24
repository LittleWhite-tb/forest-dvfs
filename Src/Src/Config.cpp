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

#include <string>
#include <fstream>
#include <vector>

#include "Config.h"
#include "Common.h"
#include "glog/logging.h"
#include "FileUtils.h"

Config::Config()
{
   loadValues();
}


Config::~Config()
{

}


void Config::loadValues()
{
   std::vector<std::string> filenames;
   std::fstream fs;
   filenames.push_back ("config.cfg");

   FileUtils::tryToOpen (filenames, fs, std::fstream::in);

   std::vector<std::string> ln;
   while (FileUtils::readLine(ln, fs))
   {
      // ignore empty lines
      if (ln.size() == 0)
      {
         continue;
      }

      // ignore comments
      if (ln[0] == "#")
      {
         ln.clear();
         continue;
      }

      // 2 tokens entries (varname value)
      if (ln.size() == 2)
      {
         DLOG(INFO) << "Found property " << ln[0] << std::endl;
         this->values.insert(std::pair<const std::string, const std::string>(ln[0], ln[1]));
      }

      // 3 tokens entries (varname = value)
      if (ln.size() == 3)
      {
         if (ln[1] != "=" && ln[1] != ":")
         {
            LOG(FATAL) << "Invalid configuration file content." << std::endl;
         }

         DLOG(INFO) << "Found property " << ln[0] << std::endl;
         this->values.insert(std::pair<const std::string, const std::string>(ln[0], ln[2]));
      }

      ln.clear();
   }

   fs.close ();
}

