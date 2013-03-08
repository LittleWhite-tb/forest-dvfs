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

#include "Config.h"
#include "Common.h"
#include "DataFileReader.h"
#include "glog/logging.h"

#include <string>
#include <vector>


Config::Config()
{
   loadValues();
}


Config::~Config()
{

}


void Config::loadValues()
{
   DataFileReader fd("config.cfg");

   if (!fd.isOpen())
   {
      LOG(FATAL) << "Configuration file not found." << std::endl;
   }

   std::vector<std::string> ln;
   while (fd.readLine(ln))
   {
      // ignore empty lines
      if (ln.size() == 0)
      {
         continue;
      }

      // ignore comments
      if (ln[0] == "#")
      {
         continue;
      }

      // 2 tokens entries (varname value)
      if (ln.size() == 2)
      {
         this->values.insert(std::pair<const std::string, const std::string>(ln[0], ln[1]));
         continue;
      }

      // 3 tokens entries (varname = value)
      if (ln.size() == 3)
      {
         if (ln[1] != "=" && ln[1] != ":")
         {
            LOG(FATAL) << "Invalid configuration file content." << std::endl;
         }

         this->values.insert(std::pair<const std::string, const std::string>(ln[0], ln[2]));
      }
   }
}

