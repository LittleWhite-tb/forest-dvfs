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
 @file FreqChanger.h
 @brief The FreqChanger class header is in this file
 */

#ifndef H_FREQCHANGER
#define H_FREQCHANGER

#include <pthread.h>
#include "CoresInfo.h"

/**
 * @class FreqChanger
 * @brief Allow the frequency changing but look out if is there is multiple
 * threads pinning because the CPU won't allow to have different core at a different frequency
 */
class FreqChanger
{
   public:
      /**
       * @brief Constructor
       */
      FreqChanger (CoresInfo * coresInfo);

      /**
       * @brief Destructor
       */
      ~FreqChanger (void);

      /**
       * @brief read the frequency of a specific core
       * @param coreId the id of the core to read
       */
      unsigned int readCurrentFreq (unsigned int coreId);

      /**
       * @brief Change the frequency at a specific one
       * @param coreId the id number of the core
       * @param freqId the freq to set the core
       */
      void changeFreq (unsigned int coreId, unsigned int freqId);

      /**
       * @brief Return all infos on cores and frequencies changing
       * @return the string containing infos
       */
      std::string debug ();

   private:
      //Variables
      CoresInfo * coresInfo; /**<@brief Save the point of the CoresInfos structure, see the class**/
      pthread_mutex_t mutex; /** @brief Mutex to lock access to data contained in  CoresInfo**/

};
#endif
