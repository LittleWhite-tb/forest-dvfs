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

#ifndef CONFIG_H
#define CONFIG_H

#include "Config.h"

#include <cstdlib>
#include <map>
#include <string>

#define GLOBAL_INT(varname) Config::getInstance().getInt(varname)
#define GLOBAL_FLOAT(varname) Config::getInstance().getFloat(varname)

class Config
{
public:
   /**
    * Default constructor.
    */
   Config();

   /**
    * Default destructor.
    */
   ~Config();

   /**
    * Returns the value of the given property as a float.
    *
    * @return The value of the requested property.
    */
   inline float getFloat(const std::string &varname)
   {
      return strtof(this->values[varname].c_str(), NULL);
   }
  
   /**
    * Returns the value of the given property as a int.
    *
    * @return The value of the requested property.
    */
   inline int getInt(const std::string &varname)
   {
      return atoi(this->values[varname].c_str());
   }


private:

   /**
    * All values read when constructed.
    */
   std::map<const std::string, const std::string> values;

   /**
    * (Re)Loads the configuration from file.
    */
   void loadValues();
};

#endif
