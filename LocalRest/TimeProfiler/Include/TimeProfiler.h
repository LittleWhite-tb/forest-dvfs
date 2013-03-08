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


/**
 * @file TimeProfiler.h
 * Profiles how REST is spending time
 */

#pragma once
#ifndef H_TIMEPROFILER

#include <time.h>
#include <stdint.h>
#include <sstream>
#include <cstring>
#include <iomanip>
#include "DecisionMaker.h"

typedef enum {       
	EVALUATION = 0,
	SEQUENCE_EXECUTION,
	STATE_SIZE
} State;

typedef enum {
	EVALUATION_INIT = 0,
	FREQUENCY_EVALUATION,
	STEPS_COMPUTATION,
	SEQUENCE_COMPUTATION,
	EVALSTATE_SIZE
} EvalState;

#define EXECUTION_SLOT (EVALSTATE_SIZE)

// TODO Comment
inline timespec operator- (const timespec &end, const timespec &start)
{
  timespec temp;
  if ((end.tv_nsec - start.tv_nsec) < 0) {
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	
	return temp;
}

// TODO comment
class TimeProfiler {
private:
	unsigned int timeRefSize;
	timespec timeRef [EVALSTATE_SIZE + 1]; // Eval States + Execution State
	timespec buffer;
public:
	TimeProfiler () :
	timeRefSize (sizeof (timeRef)/sizeof (timeRef[0])) {
		// Initializes data correctly
		memset (&buffer, 0, sizeof (buffer));
		memset (timeRef, 0, sizeof (timeRef));
	}

	~TimeProfiler () {}

	inline void evaluate (unsigned int toSet) {
		assert (toSet < timeRefSize);
		if (toSet == EXECUTION_SLOT) {
			buffer = timeRef [EXECUTION_SLOT];	
		}
		clock_gettime (CLOCK_MONOTONIC, timeRef + toSet);
	}

	inline void print (std::ostringstream& toSet) const{
		timespec ts;
		uint64_t res;
		ts = timeRef [0] - buffer;
		res = ts.tv_sec * 1000000000 + ts.tv_nsec;
		toSet << std::setw (20) << res << " ";
		for (unsigned int i = 1; i < timeRefSize; i++) {
			ts = timeRef[i] - timeRef [i-1];
			res = ts.tv_sec * 1000000000 + ts.tv_nsec;
			toSet << std::setw (20) << res << " ";
		}
	}

	inline const timespec& getTimeRef (unsigned int at) {
		assert (at < timeRefSize);
		return timeRef [at];
	}
};

#endif
