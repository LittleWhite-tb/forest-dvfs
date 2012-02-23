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

#ifndef H_CORESINFO
#define H_CORESINFO

#include <vector>
#include <fstream>
#include <string>

/**
 * @brief structure useful for each core
 */
typedef struct coreData
{
		std::ofstream * freqFd;    /**< @brief where to write the frequencies */
		unsigned long int * freqTrack;   /**< @brief track the frequencies values changes*/
		unsigned int requestedFreq;      /**< @brief The last requested frequency */
		std::string oldGov;        /**< @brief original governor registered for this core. */
} CoreData;

/**
 * @class CoresInfo
 * @brief
 */
class CoresInfo
{
	public:
		/**
		 * @brief Constructor
		 */
		CoresInfo (void);

		/**
		 * @brief Destructor
		 */
		~CoresInfo (void);

		CoreData * allCoreData; /**< @brief the available frequencies on the system of size numFreq*/
		unsigned int numCores;  /**< @brief number of available cores**/

		// Limits valid for the whole system
		int freqMax;   /**< @brief the highest available frequencies*/
		int freqMin;   /**< @brief the lowest available frequencies*/
		unsigned int numFreqs;  /**< @brief the number of available frequencies*/
		unsigned int * availableFreqs;   /**< @brief the available frequencies on the system of size numFreq sorted in decreasing order*/
      bool hasTurboBoost;  /**< @brief True when the processor has a TurboBoost mode. */

	private:

		/**
		 * @brief Learn about the hardware performances
		 */
		void initCpuData ();

		/**
		 * @brief Initialize Structure containing important information for rest for a specific core
		 * @param coreId the specific core
		 * @param data the core data to initialize
		 */
		void initCoreData (unsigned int coreId, CoreData & data);

		/**
		 * @brief Cleanups the data and execution context related to the given
		 * core.
		 *
		 * @param coreId The id of the concerned core.
		 */
		void freeCoreData (unsigned int coreId);
};
#endif
