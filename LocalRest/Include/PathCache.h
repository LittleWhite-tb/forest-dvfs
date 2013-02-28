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
 
#ifndef PATHCACHE_H
#define PATHCACHE_H

#include <map>
#include <string>

class NoCache
{
public:

   bool exists(const unsigned int key)const { (void)key; return false; }
   bool get(const unsigned int key, std::string& path)const  { (void)key; (void)path; return false; }
   void store(const unsigned int key, const std::string& path)  { (void)key; (void)path; }
};

class PathCache
{
private:
   typedef std::map<unsigned int, std::string> CacheType;
   CacheType m_cache;
   
public:

   bool exists(const unsigned int key)const;
   bool get(const unsigned int key, std::string& path)const;
   void store(const unsigned int key, const std::string& path);
};

#endif
