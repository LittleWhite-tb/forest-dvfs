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
  @file CoresInfos.h
  @brief The CoresInfos class header is in this file
 */

#ifndef H_CORESINFOS
#define H_CORESINFOS

#include <vector>
#include <fstream>

/**
 * @brief structure useful for each core
 */
typedef struct coreData
{
        unsigned int idCore;    /**< @brief the id of the core associated to its profile informations*/
        std::ofstream *freq_fd;  /**< @brief where to write the frequencies */
        int currentFreq;	    /**< @brief int that holds the currentFrequency */
        unsigned long int *freqTrack;  /**< @brief track the frequencies values changes*/
} CoreData;



/**
 * @class CoresInfos
 * @brief 
 */
class CoresInfos
{
	public:
		/**
		 * @brief Constructor
		 */
		CoresInfos (void);
		
		/**
		 * @brief Destructor
		 */
		~CoresInfos (void);

        CoreData *all_core_data; /**< @brief the available frequencies on the system of size numFreq*/

		//variables
		int freqMax;			/**< @brief the highest available frequencies*/
		int freqMin;			/**< @brief the lowest available frequencies*/
		unsigned int numFreqs;	/**< @brief the number of available frequencies*/
		unsigned int numCores;	/**< @brief number of available cores**/
		int *availableFreqs;   /**< @brief the available frequencies on the system of size numFreq*/

    private:

		/**
		 * @brief Learn about the hardware performances
		 */
		void initCpuDatas ();

		/**
		 * @brief Initialize Structure containing important information for rest for a specific core
		 * @param coreId the specific core
		 * @return the core datas initialized
		 */
		CoreData initCoreDatas(unsigned int coreId);
};
#endif
