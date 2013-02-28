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
 
#include "PathCache.h"

#include <iostream>

bool PathCache::exists(const unsigned int key)const
{
   return m_cache.find(key) != m_cache.end();
}

bool PathCache::get(const unsigned int key, std::string& path)const
{
   CacheType::const_iterator itElem = m_cache.find(key);
   if ( itElem != m_cache.end() )
   {
      path = itElem->second;
      return true;
   }
   return false;
}

void PathCache::store(const unsigned int key, const std::string& path)
{
   if ( exists(key) )
   {
      std::cerr << "WARNING : PathCache have already cached a path for key " << key << " (overwritting)" << std::endl;
   }
   
   m_cache[key]=path;
}
