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
#include <string>

/**
 * @brief structure useful for each core
 */
typedef struct coreDatas
{
        int idCore;			/**< @brief the id of the core associated to its profile informations*/
        std::string setFile;		/**< @brief pointer to an array of File descriptors of size numCores used to change frequency */
        int currentFreq;		/**< @brief int that holds the currentFrequency for */
        std::vector<long int> freqTrack;/**< @brief track the frequencies values changes*/
} CoreDatas;



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

		/**
		 * @brief Learn about the hardware performances
		 */
		void initCpuDatas ();

		/**
		 * @brief Initialize Structure containing important information for rest for a specific core
		 * @param coreId the specific core
		 * @return the core datas initialized
		 */
		CoreDatas initCoreDatas(int coreId);

		std::vector<CoreDatas> *coreDatasArray; /**< @brief the available frequencies on the system of size numFreq*/

		//variables
		int freqMax;			/**< @brief the highest available frequencies*/
		int freqMin;			/**< @brief the lowest available frequencies*/
		int numFreqs;			/**< @brief the number of available frequencies*/
		int numCores;			/**< @brief number of available cores**/
		std::vector<int> availableFreqs;/**< @brief the available frequencies on the system of size numFreq*/
};
#endif
