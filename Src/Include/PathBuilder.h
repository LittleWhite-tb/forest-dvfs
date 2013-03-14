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

#ifndef PATHBUILDER_H
#define PATHBUILDER_H

#include <string>
#include <sstream>
#include <cassert>

#include "PathCache.h"

enum PathType
{
   PT_CPUINFO_TRANSITION_LATENCY,
   PT_CPUINFO_RELATED_CPU,
   PT_CPUINFO_SCALING_AVAIL_FREQS,
   PT_CPUINFO_SCALING_GOVERNOR,
   PT_CPUINFO_SCALING_SETSPEED,
   PT_TOPOLOGY_CORE_ID,
   PT_TOPOLOGY_PKG_ID,
   PT_POWER_CONFIG,
};

template<PathType PT,typename CachingMethod>
class PathBuilder
{   
public:
   static std::string buildPath (const unsigned int cpuID);
};

template <typename CachingMethod>
class PathBuilder<PT_CPUINFO_TRANSITION_LATENCY,CachingMethod>
{
private:
   static CachingMethod m_cache;
   
public:
   static std::string buildPath (const unsigned int cpuID)
   {
      std::string path;
      if ( m_cache.get (cpuID,path))
      {
      }
      else
      {
         std::ostringstream oss;
         oss << "/sys/devices/system/cpu/cpu" << cpuID << "/cpufreq/cpuinfo_transition_latency";
         return oss.str ();
         
         m_cache.store (cpuID,path);
      }
      return path;
   }
};

template <typename CachingMethod>
class PathBuilder<PT_CPUINFO_RELATED_CPU,CachingMethod>
{
private:
   static CachingMethod m_cache;
   
public:
   static std::string buildPath (const unsigned int cpuID)
   {
      std::string path;
      if ( m_cache.get (cpuID,path))
      {
      }
      else
      {
         std::ostringstream oss;
         oss << "/sys/devices/system/cpu/cpu" << cpuID << "/cpufreq/related_cpus";
         path = oss.str ();
         
         m_cache.store (cpuID,path);
      }
      return path;
   }
};

template <typename CachingMethod>
class PathBuilder<PT_CPUINFO_SCALING_AVAIL_FREQS,CachingMethod>
{
private:
   static CachingMethod m_cache;
   
public:
   static std::string buildPath (const unsigned int cpuID)
   {
      std::string path;
      if ( m_cache.get (cpuID,path))
      {
      }
      else
      {
         std::ostringstream oss;
         oss << "/sys/devices/system/cpu/cpu" << cpuID << "/cpufreq/scaling_available_frequencies";
         path = oss.str ();
         
         m_cache.store (cpuID,path);
      }
      return path;
   }
};

template <typename CachingMethod>
class PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,CachingMethod>
{
private:
   static CachingMethod m_cache;
   
public:
   static std::string buildPath (const unsigned int cpuID)
   {
      std::string path;
      if ( m_cache.get (cpuID,path))
      {
      }
      else
      {
         std::ostringstream oss;
         oss << "/sys/devices/system/cpu/cpu" << cpuID << "/cpufreq/scaling_governor";
         path = oss.str ();
         
         m_cache.store (cpuID,path);
      }
      return path;
   }
};

template <typename CachingMethod>
class PathBuilder<PT_CPUINFO_SCALING_SETSPEED,CachingMethod>
{
private:
   static CachingMethod m_cache;
   
public:
   static std::string buildPath (const unsigned int cpuID)
   {
      std::string path;
      if ( m_cache.get (cpuID,path))
      {
      }
      else
      {
         std::ostringstream oss;
         oss << "/sys/devices/system/cpu/cpu" << cpuID << "/cpufreq/scaling_setspeed";
         path = oss.str ();
         
         m_cache.store (cpuID,path);
      }
      return path;
   }
};

template <typename CachingMethod>
class PathBuilder<PT_TOPOLOGY_CORE_ID,CachingMethod>
{
private:
   static CachingMethod m_cache;
   
public:
   static std::string buildPath (const unsigned int cpuID)
   {
      std::string path;
      if ( m_cache.get (cpuID,path))
      {
      }
      else
      {
         std::ostringstream oss;
         oss << "/sys/devices/system/cpu/cpu" << cpuID << "/topology/core_id";
         path = oss.str ();
         
         m_cache.store (cpuID,path);
      }
      return path;
   }
};


template <typename CachingMethod>
class PathBuilder<PT_TOPOLOGY_PKG_ID,CachingMethod>
{
private:
   static CachingMethod m_cache;
   
public:
   static std::string buildPath (const unsigned int cpuID)
   {
      std::string path;
      if ( m_cache.get (cpuID,path))
      {
      }
      else
      {
         std::ostringstream oss;
         oss << "/sys/devices/system/cpu/cpu" << cpuID << "/topology/physical_package_id";
         path = oss.str ();
         
         m_cache.store (cpuID,path);
      }
      return path;
   }
};


template <typename CachingMethod>
class PathBuilder<PT_POWER_CONFIG,CachingMethod>
{
private:
   static CachingMethod m_cache;
   
public:
   static std::string buildPath (const unsigned int cpuID)
   {
      std::string path;
      if ( m_cache.get (cpuID,path))
      {
      }
      else
      {
         std::ostringstream oss;
         oss << "offline/power_" << cpuID << ".cfg";
         path = oss.str ();
         
         m_cache.store (cpuID,path);
      }
      return path;
   }
};

// Static declaration
template <typename CachingMethod>
CachingMethod PathBuilder<PT_CPUINFO_TRANSITION_LATENCY,CachingMethod>::m_cache=CachingMethod ();
template <typename CachingMethod>
CachingMethod PathBuilder<PT_CPUINFO_RELATED_CPU,CachingMethod>::m_cache=CachingMethod ();
template <typename CachingMethod>
CachingMethod PathBuilder<PT_CPUINFO_SCALING_AVAIL_FREQS,CachingMethod>::m_cache=CachingMethod ();
template <typename CachingMethod>
CachingMethod PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,CachingMethod>::m_cache=CachingMethod ();
template <typename CachingMethod>
CachingMethod PathBuilder<PT_CPUINFO_SCALING_SETSPEED,CachingMethod>::m_cache=CachingMethod ();
template <typename CachingMethod>
CachingMethod PathBuilder<PT_TOPOLOGY_CORE_ID,CachingMethod>::m_cache=CachingMethod ();
template <typename CachingMethod>
CachingMethod PathBuilder<PT_TOPOLOGY_PKG_ID,CachingMethod>::m_cache=CachingMethod ();
template <typename CachingMethod>
CachingMethod PathBuilder<PT_POWER_CONFIG,CachingMethod>::m_cache=CachingMethod ();

#endif
