/*
 * FoREST - Reactive DVFS Control for Multicore Processors
 * Copyright (C) 2013 Universite de Versailles
 * Copyright (C) 2011-2012 Exascale Research Center
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

#ifndef H_LOGGER
#define H_LOGGER

#include <fstream>
#include <vector>

/**
 * Logger class based on Singleton pattern and allowing to handle N logs
 * The number of logs is determined at the construction
 * \todo this class should be split with a second class "Log"
 */
class Logger
{
	public:
		
      /**
       * Init the logs
       * \param nbTh number of logs to create
       */
		static void initLog (unsigned int nbTh);
      
      /**
       * Free all logs
       * This function has to be called at program's end
       */
		static void destroyLog (void);
      
      /**
       * Get a specific log
       * \param id identifier to the log to get
       * \return pointer to the logger
       */
		static Logger *getLog (unsigned int id);

      /**
       * Write in the log
       * \param str string to log
       */
		void logOut (const char *str);
      
      /**
       * Write in the log
       * \param oss stream to log
       */
		void logOut (std::ostringstream& oss);
			
      /**
       * Write block ender
       */
		void endBlock (void);
 		
		

	private:
      static std::vector<Logger *> logList;  /*!< Logs container */
   
		std::ofstream switchOFS;   /*!< log file handler */
		unsigned int thId;         /*!< log id */
      
      /**
       * \param id identifier to use for the new log
       */
      Logger(unsigned int id);
      
      /**
       * Close the log
       */
      ~Logger ();
	
};
#endif
