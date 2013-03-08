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

#ifndef DATAFILEREADER_HPP
#define DATAFILEREADER_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

class DataFileReader
{
private:

   std::ifstream m_file;

public:
   DataFileReader (const std::string& filePath);
   ~DataFileReader ();
   
   bool isOpen ()const;
   
   template<typename T>
   bool readLine (std::vector<T>& data)
   {
      std::string line;
      std::getline (this->m_file, line, '\n');
      std::istringstream iss (line);
      T tmp = T ();

      iss >> std::ws;
      
      while (iss >> tmp >> std::ws) 
      {
         data.push_back (tmp);
      }
      
      return this->m_file;
   }
   
   template<typename T>
   bool read (T& data)
   {
      m_file >> data;
      return m_file;
   }

};

#endif
