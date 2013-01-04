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
 * @file NewAdaptiveDecisions.cpp
 * The NewAdaptiveDecisions class is in this file
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <stdint.h>
#include <cassert>
#include "NewAdaptiveDecisions.h"
#include "Common.h"
#include <math.h>
#include "FreqSelector.h"

#ifdef REST_LOG
	#include "Logger.h"
	#include <iomanip>
#endif

NewAdaptiveDecisions::NewAdaptiveDecisions (const DVFSUnit& unit) :
   DecisionMaker (unit),
	 timeProfiler (),
	 freqSelector (unit.getNbFreqs ())
{
	this->curRuntimeState = EVALUATION;
	this->curEvalState = EVALUATION_INIT;
	this->currentSeqChunk = 0;
	this->totalsleepWin = NewAdaptiveDecisions::MIN_SLEEP_WIN;
	this->prof = 0;
#ifdef REST_LOG
	this->log = Logger::getLog(this->unit.getId ());
#endif

	this->nbCpuIds = unit.getNbCpuIds ();
	this->nbFreqs = unit.getNbFreqs ();
	assert (this->nbCpuIds != 0);
	assert (this->nbFreqs != 0);

	this->ipcEvalSize = this->nbFreqs * this->nbCpuIds;

	// Allocate data
	this->stepState = new short int [this->nbCpuIds];
	this->ipcEval = new float [this->ipcEvalSize];
	this->dec = new DecisionCouple [this->nbCpuIds];
	this->evaluateFreq = new bool [this->nbFreqs];
	this->timeRatios = new float [this->nbFreqs];

	// Check that allocation was successful
	handleAllocation (this->stepState);
	handleAllocation (this->ipcEval);
	handleAllocation (this->dec);
	handleAllocation (this->evaluateFreq);
	handleAllocation (this->timeRatios);

	// Reserve the critical vectors for performance reasons
	this->sequence.reserve (this->nbCpuIds+1);
	this->freqsToEvaluate.reserve (this->nbFreqs);

	// Init memory
	//memset (this->dec, 0, this->nbCpuIds*sizeof (*this->dec));
	
	this->oldMaxFreqId = 0;
}

NewAdaptiveDecisions::~NewAdaptiveDecisions (void)
{
	delete [] this->ipcEval, this->ipcEval = 0;
	delete [] this->dec, this->dec = 0;
	delete [] this->stepState, this->stepState = 0;
	delete [] this->evaluateFreq, this->evaluateFreq = 0;
	delete [] this->timeRatios, this->timeRatios = 0;
}

void NewAdaptiveDecisions::getVirtualFreq (float degradedIPC, unsigned int cpu) {
	assert (cpu < this->nbCpuIds);
	Decision& Step1 = this->dec[cpu].step[STEP1];
	Decision& Step2 = this->dec[cpu].step[STEP2];
	float max = 0;
	float min = 100.;
	
	int tmpfreqId1 = -1;
	int tmpfreqId2 = -1;

	Step1.freqId = 0;
	Step2.freqId = 0;
	Step1.timeRatio = 0.0;
	Step2.timeRatio = 0.0;

	unsigned int freqVecSize = this->freqsToEvaluate.size ();
	// find the two consecutive IPC that majorate and minorate the degradedIPC
	for (unsigned int i = 0; i < freqVecSize; i++) {
		unsigned int baseIdx = this->freqsToEvaluate[i] * this->nbCpuIds;
		unsigned int idx = baseIdx + cpu;
		if (max < this->ipcEval[idx] && this->ipcEval[idx] < degradedIPC) {
			max = this->ipcEval[idx];
			tmpfreqId1 = this->freqsToEvaluate[i];	
		}
		if (min > this->ipcEval[idx] && this->ipcEval[idx] > degradedIPC) {
			min = this->ipcEval[idx];
			tmpfreqId2 = this->freqsToEvaluate[i];
		}
	}

	if (tmpfreqId1 == -1) {
		Step2.timeRatio = 1;
		if (tmpfreqId2 == -1) {
			Step2.freqId = 0;
			assert (degradedIPC == 0);
			//std::cerr << "Setting 0 exceptionally" << std::endl;
		} else {
			Step2.freqId = tmpfreqId2;
		}

		Step1.timeRatio = 0;
		Step1.freqId = 0;
	} else {
		assert (tmpfreqId2 != -1);
		//once the freq are found, compute the time to spend in each
		Step2.freqId = tmpfreqId2;
		Step2.timeRatio = (degradedIPC - max)/(min-max);

		Step1.freqId = tmpfreqId1;
		Step1.timeRatio = 1 - Step2.timeRatio;	
	}
	assert (Step1.freqId < this->nbFreqs);
	assert (Step2.freqId < this->nbFreqs);
	assert (Step1.timeRatio <= 1);
	assert (Step2.timeRatio <= 1);
}

void NewAdaptiveDecisions::logFrequency (unsigned int freqId) const{	
	assert (freqId < this->nbFreqs);
	#ifdef REST_LOG	
		std::ostringstream logger;
		logger << freqId;

#if 0
	for (unsigned int i = 0; i < this->sequence.size (); i++) {
		logger << "{" << this->sequence[i].freqId << ", " << this->sequence [i].timeRatio << "}, ";	
	}
	logger << "}" << std::endl;	


		float ipcMax = 0.;
		unsigned int max = 0;

		for (unsigned int i = 1; i < this->ipcEvalSize; i++) {
			ipcMax = rest_max (ipcMax, this->ipcEval [i]);
			if (this->ipcEval [i] > this->ipcEval [max]) {
				max = i;
			}
		}
		/* modulo : core
		 * division : freq selection (ou inversement)
		 */
		logger << " " << this->ipcEval [max] << std::endl;

	logger << "IPC Eval array :" << std::endl;
	unsigned int freqsToEvalSize = this->freqsToEvaluate.size ();
	logger << std::endl;
	for (unsigned int i = 0; i < freqsToEvalSize; i++) {
		unsigned int baseIdx = this->freqsToEvaluate [i]*this->nbCpuIds;
		logger << this->freqsToEvaluate [i] << " ";
		for (unsigned int j = 0; j < this->nbCpuIds; j++) {
			logger << std::setw (10) << this->ipcEval[baseIdx+j] << " ";
		}	
		logger << std::endl;
	}


	// Debug print
	for (unsigned int i = 0; i < this->nbCpuIds; i++) {
		logger << "Core #" << i << ":" << std::endl;
		logger << "\tStep1 {" << this->dec[i].step[STEP1].freqId << ", " << this->dec[i].step[STEP1].timeRatio << "}" << std::endl;
		logger << "\tStep2 {" << this->dec[i].step[STEP2].freqId << ", " << this->dec[i].step[STEP2].timeRatio << "}" << std::endl;
	}
#endif
	this->log->logOut (logger);

	#endif
	(void) freqId;
}

// TODO comment
void NewAdaptiveDecisions::lengthenSleepTime () {
	unsigned int maxRatioId = 0;
	
	for (size_t i = 1; i < this->nbFreqs; i++) {
		if (this->timeRatios [maxRatioId] < this->timeRatios [i]) {
			maxRatioId = i;
		}
	}

	if (this->freqSelector.areWeStableYet (maxRatioId)) {
		//std::cerr << "Stable yeti #" << freqId << " : " << freqSelector.getFrequencyRatio (freqId) << std::endl;
		this->totalsleepWin = rest_min (NewAdaptiveDecisions::MAX_SLEEP_WIN, this->totalsleepWin*2);
	} else {
		unsigned int minFreqWindow = rest_max (this->oldMaxFreqId - NewAdaptiveDecisions::MAX_FREQ_WINDOW, 0);
		unsigned int maxFreqWindow = this->oldMaxFreqId + NewAdaptiveDecisions::MAX_FREQ_WINDOW;

		//std::cerr << "OldMaxFreqId = " << this->oldMaxFreqId << ", Freqid = " << freqId << ", minWin = " << minFreqWindow << ", maxWin = " << maxFreqWindow << std::endl;
		
		if (maxRatioId < minFreqWindow || maxRatioId > maxFreqWindow) {
			//std::cerr << "Effectively reset" << std::endl;
			//std::cerr << "Not stable yet :#" << freqId << " : " << freqSelector.getFrequencyRatio (freqId) << std::endl;
			this->totalsleepWin = NewAdaptiveDecisions::MIN_SLEEP_WIN;
			this->logFrequency (maxRatioId);
		} else {
			//std::cerr << "Too little difference" << std::endl;
		}
	}
	this->oldMaxFreqId = maxRatioId;
}

Decision NewAdaptiveDecisions::initEvaluation () {
	Decision res = DECISION_DEFAULT_INITIALIZER;
	// Reset the list of frequencies to evaluate
	this->freqsToEvaluate.clear ();
	
	// Reinit memory
	memset (this->evaluateFreq, 0, this->nbFreqs*sizeof (*this->evaluateFreq));
	
	//memset (this->ipcEval, 0, this->ipcEvalSize*sizeof (*this->ipcEval));

	for (unsigned int i = 0; i < this->nbCpuIds; i++) {
		// Computing the freq window for the current core
		this->computeFreqWindowCenter (i);
		this->computeFreqWindow ();
	}
	
	// Now we compute
	for (unsigned i = 0; i < this->nbFreqs; i++) {
		if (this->evaluateFreq[i]) {
			//std::cerr << "Pushing " << i << std::endl;
			this->freqsToEvaluate.push_back (i);
		}
	}
	
	assert (this->freqsToEvaluate.size () <= this->nbFreqs);
	assert (this->freqsToEvaluate.size () > 0);
	
	// Prepare the first frequency to be the minimum of our freshly computed frequency window
	this->currentFreqId = 0;
	
	// We're all set ! Let's evaluate the frequencies !
	this->curEvalState = FREQUENCY_EVALUATION;

	res.freqId = this->freqsToEvaluate [0];
	res.sleepWin = NewAdaptiveDecisions::IPC_EVAL_TIME;	
	res.freqApplyDelay = this->unit.getSwitchLatency () / 1000;

	this->timeProfiler.evaluate (EVALUATION_INIT);
	return res;
}

Decision NewAdaptiveDecisions::evaluateFrequency () {
	assert (this->currentFreqId < this->freqsToEvaluate.size ());
	assert (this->prof != 0);

	unsigned int currentFreq = this->freqsToEvaluate[this->currentFreqId];
	unsigned int freqVecSize = this->freqsToEvaluate.size ();
	bool hwcPanic = false; // Decides whether the HWC values are reasonable
	
	for (unsigned int i = 0; i < this->nbCpuIds; i++) {
		HWCounters hwc;
		this->prof->read (hwc, i);
		float HWexp = this->getHWExploitationRatio (hwc);
		//std::cerr << "#" << i << ", freq #" << currentFreq << " IPC = " << HWexp << std::endl;
		if (HWexp < 1e-3 || HWexp > 15 || isnan (HWexp)) {
			hwcPanic = true;
		} else {
			this->ipcEval [currentFreq*this->nbCpuIds + i] = HWexp;
		}
	}
	
	if (!hwcPanic && this->currentFreqId+1 == freqVecSize) {
		this->curEvalState = STEPS_COMPUTATION;
		this->timeProfiler.evaluate (FREQUENCY_EVALUATION);
		Decision res = DECISION_ZERO_INITIALIZER;
		return res;
	} else {
		Decision res = DECISION_DEFAULT_INITIALIZER;
		if (!hwcPanic) {
			this->currentFreqId++;
		}
		res.freqId = this->freqsToEvaluate[this->currentFreqId];
		res.sleepWin = NewAdaptiveDecisions::IPC_EVAL_TIME;
		res.freqApplyDelay = this->unit.getSwitchLatency () / 1000;
		return res;
	}
}

void NewAdaptiveDecisions::computeSteps () {
#if 0
	std::cerr << "IPC Eval array :" << std::endl;
	for (unsigned int i = 0; i < this->nbFreqs; i++) {
		if (this->evaluateFreq[i]) {
						std::cerr << "{ ";
		}
		for (unsigned int j = 0; j < this->nbCpuIds; j++) {
						std::cerr << this->ipcEval[i*this->nbCpuIds+j] << " ";
		}
		if (this->evaluateFreq[i]) {
						std::cerr << " }";
		}
		std::cerr << std::endl;
	}
#endif

	// get the maxIPC from the tested IPC
	unsigned int freqVecSize = this->freqsToEvaluate.size ();	

	for (unsigned int cpu = 0; cpu < this->nbCpuIds; cpu++) {
		float maxIPC = 0;
		for (unsigned int i = 0; i < freqVecSize; i++) {
			unsigned int baseIdx = this->freqsToEvaluate.at(i) * this->nbCpuIds;
			maxIPC = rest_max (maxIPC, this->ipcEval [baseIdx + cpu]);
		}

		float degradedIPC = 0.95 * maxIPC;
		this->getVirtualFreq (degradedIPC, cpu);
	}
	
	this->curEvalState = SEQUENCE_COMPUTATION;
	this->timeProfiler.evaluate (STEPS_COMPUTATION);
}

//#if 0
void NewAdaptiveDecisions::computeSequence () {
	// Reinit data
	memset (this->timeRatios, 0, this->nbFreqs*sizeof (*this->timeRatios));
	
	// Compute time ratios
	for (unsigned int i = 0; i < this->nbCpuIds; i++) {
		Decision& step1 = this->dec [i].step [STEP1];
		Decision& step2 = this->dec [i].step [STEP2];

		this->timeRatios [step1.freqId] = rest_max (this->timeRatios [step1.freqId], step1.timeRatio);
		this->timeRatios [step2.freqId] = rest_max (this->timeRatios [step2.freqId], step2.timeRatio);
	}

	// Promote frequencies and search for max time ratio
	unsigned int maxRatioFreqId = 0;
	for (unsigned int i = 0; i < this->nbFreqs; i++) {
		if (this->timeRatios [i] != 0) {
			this->freqSelector.promote (i, this->timeRatios [i]);
		}

		if (this->timeRatios [maxRatioFreqId] < this->timeRatios [i]) {
			maxRatioFreqId = i;
		}
	}

	// Lengthen sleeping time if necessary
	if (this->freqSelector.areWeStableYet (maxRatioFreqId)) {
		this->totalsleepWin = rest_min (NewAdaptiveDecisions::MAX_SLEEP_WIN, this->totalsleepWin*2);
	} else {
		unsigned int minFreqWindow = rest_max (this->oldMaxFreqId - NewAdaptiveDecisions::MAX_FREQ_WINDOW, 0);
		unsigned int maxFreqWindow = this->oldMaxFreqId + NewAdaptiveDecisions::MAX_FREQ_WINDOW;
		
		if (maxRatioFreqId < minFreqWindow || maxRatioFreqId > maxFreqWindow) {	
			this->totalsleepWin = NewAdaptiveDecisions::MIN_SLEEP_WIN;
			this->logFrequency (maxRatioFreqId);
		}
	}	

	this->sequence.clear ();

	// Generate sequence
	float totalTimeRatio = 0.;
	int currentFreq = this->nbFreqs - 1;
	while (totalTimeRatio < 1 && currentFreq >= 0) {
		if (this->timeRatios [currentFreq] * this->totalsleepWin > NewAdaptiveDecisions::MIN_SLEEP_WIN) {
			FreqChunk freqChunk;
			freqChunk.freqId = currentFreq;
			if (this->timeRatios [currentFreq] + totalTimeRatio > 1) {
				freqChunk.timeRatio = 1 - totalTimeRatio;
			} else {
				freqChunk.timeRatio = this->timeRatios [currentFreq];
			}
			this->sequence.push_back (freqChunk);
			totalTimeRatio += freqChunk.timeRatio;
		}
		currentFreq--;
	}
	
	// Special case : Any frequency is significant enough, so we take the previous choice as the good one
	if (totalTimeRatio < 1) {	
		FreqChunk freqChunk = { maxRatioFreqId, 1};
		this->sequence.clear ();
		this->sequence.push_back (freqChunk);
	}

#if 0
	std::cerr << "#" << this->unit.getId () << ", st = " << this->totalsleepWin << " {";
	for (unsigned int i = 0; i < this->sequence.size (); i++) {
		std::cerr << "{" << this->sequence[i].freqId << ", " << this->sequence [i].timeRatio << "}, ";	
	}
	std::cerr << "}" << std::endl;
#endif	

	this->oldMaxFreqId = maxRatioFreqId;	
}
//#endif

#if 0
void NewAdaptiveDecisions::computeSequence () {	
	FreqChunk freqChunk; // A data structures containing the current frequency id and the time ratio. It is copied in the sequence vector
	unsigned int minRatioId = 0; // current ratio id representing the timeRatio of the current sequency chunk we are adding in the sequence
	unsigned int maxFreqId = 0; // current frequency id representing the maximum of the frequency we can choose in the current sequency chunk we are adding in the sequence
	unsigned int oldMin; // A copy of the previous minimum value that is useful in the algorithm	
	bool lastIteration = false; // The last chunk of frequency is a special case ! ;)
	unsigned int nbCpuTreated = 0; // Number of cores that have been treated : Allow to know when we're done with the computation
	float acc = 0.;  // Accumulator containing all the irrelevant timeRatios. The accumulated time ratio will be distributed among the sequence elements at the end of the computation

	this->sequence.clear ();
#if 0
	// Debug print
	for (unsigned int i = 0; i < this->nbCpuIds; i++) {
		std::cerr << "Core #" << i << ":" << std::endl;
		std::cerr << "\tStep1 {" << this->dec[i].step[STEP1].freqId << ", " << this->dec[i].step[STEP1].timeRatio << "}" << std::endl;
		std::cerr << "\tStep2 {" << this->dec[i].step[STEP2].freqId << ", " << this->dec[i].step[STEP2].timeRatio << "}" << std::endl;
	}
#endif

	memset (this->stepState, 0, sizeof (*this->stepState)*this->nbCpuIds); 
	
	// Compute the minimum time ratio id of all the decisions we took earlier
	for (unsigned int i = 1; i < this->nbCpuIds; i++) {
		if (this->dec[i].step[STEP1].timeRatio < this->dec[minRatioId].step[STEP1].timeRatio) {
			minRatioId = i;
		}

		if (this->dec[i].step[STEP1].freqId > this->dec[maxFreqId].step[STEP1].freqId) {
			maxFreqId = i;
		}
	}
	
	// Compute the maximum of all the frequencies of the first steps
	for (unsigned int i = 1; i < this->nbCpuIds; i++) {
		
	}
	
	// We store the first frequency step in the sequence
	freqChunk.timeRatio = this->dec[minRatioId].step[STEP1].timeRatio;
	freqChunk.freqId = this->dec[maxFreqId].step[STEP1].freqId;

	this->timeRatios [freqChunk.freqId] += freqChunk.timeRatio;
	this->freqSelector.promote (freqChunk.freqId, freqChunk.timeRatio);

	// In the next steps, we want to handle the second step of the minimum timeRatio (because we have just handled its first step) 
  this->stepState[minRatioId] = STEP2;
	oldMin = minRatioId;
	
	do {	
		// Initializes the minRatioId to the first cpu id that has not been handled yet
		unsigned int i;
		minRatioId = this->nbCpuIds; // We initialize to an impossible value (in order to check it has been set properly afterwards)
		for (i = 0; i < this->nbCpuIds; i++) {
			if (this->stepState[i] == STEP1) { // search for a cpu id that hasn't been handled yet
				minRatioId = i;
				break;
			}
		}
		// The minRatioId must have changed in the previous loop
		assert (minRatioId < this->nbCpuIds);
	
		// Now we're searching for the min time ratio in the remaining cpus
		for ( ; i < this->nbCpuIds; i++) {
			if (this->stepState[i] == STEP1 // Has the first ratio been handled yet ?
				&& this->dec[i].step[STEP1].timeRatio < this->dec[minRatioId].step[STEP1].timeRatio) { // Is it the minimum ?
				minRatioId = i;
			}
		}	
	
		// We compute the max of the frequencies that are between oldMin and minRatioId for step 1 first
		
		// Init maxFreqId to the first CPU that hasn't been handled yet
		// Because we know there is at least one remaining step1
		for (i = 0; i < this->nbCpuIds; i++) {
			if (this->stepState[i] == STEP1) {
				maxFreqId = i;
				break;
			}
		}
		
		// Then we want to search the max freq id along the first and second steps of the != cores.
		// We go through the first steps first
		for ( ; i < this->nbCpuIds; i++) {
			if (this->stepState[i] == STEP1
				&& this->dec[i].step[STEP1].freqId > this->dec[maxFreqId].step[STEP1].freqId) {
				maxFreqId = i;
			}
		}
		
		// Now, we have the max for step 1, but some second steps are included in the process !
		
		// We have to keep track of the current state of the transitional maxFreqId
		// Is it a first step or a second step maximum ?
		short int maxStepState = this->stepState [maxFreqId];

		for (unsigned int i = 0; i < this->nbCpuIds; i++) {
			if (this->stepState[i] == STEP2) {
				if (this->dec[i].step[STEP2].freqId > this->dec[maxFreqId].step[maxStepState].freqId) { // Is it the maximum ?
					maxFreqId = i;
					maxStepState = STEP2; // We have to update the maximum stepState !
				}
			}
		}
		assert (this->dec[maxFreqId].step[maxStepState].freqId < this->nbFreqs);

		// Now we have the minimum time ratio and the maximum frequency in this time window
		// So we can deduce the ideal frequency chunk, and we'll add it in the frequency sequence
		freqChunk.timeRatio = this->dec[minRatioId].step[STEP1].timeRatio - this->dec[oldMin].step[STEP1].timeRatio;
		freqChunk.freqId = this->dec[maxFreqId].step[maxStepState].freqId;

		this->timeRatios [freqChunk.freqId] += freqChunk.timeRatio;
		this->freqSelector.promote (freqChunk.freqId, freqChunk.timeRatio);
	
		// Ok, we have handled the first step of the minimum time ratio ; we can now handle its next step
		this->stepState[minRatioId] = STEP2;

		// And increment the number of cores's first steps treated
		nbCpuTreated++;

		// If we have treated all the first steps of all the cores, we can go in our last chunk loop iteration !
		if (nbCpuTreated+1 == this->nbCpuIds) {
			lastIteration = true;
		}
		
		// oldMin for the next iteration will be the current minimum !
		oldMin = minRatioId;
		//std::cerr << std::endl;
	} while (!lastIteration); // We end the loop to handle the special last iteration computation
	
	// We compute the max of our last time ratio frequence
	maxFreqId = 0;
	for (unsigned int i = 1; i < this->nbCpuIds; i++) {
		if (this->dec[i].step[STEP2].freqId > this->dec[maxFreqId].step[STEP2].freqId) {
			maxFreqId = i;
		}
	}
	assert (maxFreqId < this->nbCpuIds);

	freqChunk.timeRatio = this->dec[minRatioId].step[STEP2].timeRatio;
	freqChunk.freqId = this->dec[maxFreqId].step[STEP2].freqId;
		
	this->timeRatios [freqChunk.freqId] += freqChunk.timeRatio;
	this->freqSelector.promote (freqChunk.freqId, freqChunk.timeRatio);

	// Check if the new sequence is similar to the last one and lengthen the sleep time if necessary
	this->lengthenSleepTime ();

	// Generates the sequence
	for (unsigned int i = 0; i < this->nbFreqs; i++) {
		//std::cerr << "this->timeRatios[" << i << "] = " << this->timeRatios [i] << std::endl;
		//std::cerr << this->timeRatios [i] << " " << this->totalsleepWin << " " << std::endl;
		if (this->timeRatios [i]*this->totalsleepWin > NewAdaptiveDecisions::MIN_SLEEP_WIN) {
			freqChunk.timeRatio = this->timeRatios [i];
			freqChunk.freqId = i;
			this->sequence.push_back (freqChunk);
		} else {
			acc += this->timeRatios [i];
		}
	}

	unsigned int seqSize = this->sequence.size ();	
	// Special case : If we have no entry in the sequence yet
	if (seqSize == 0) {	
		freqChunk.timeRatio = 1;
		freqChunk.freqId = this->oldMaxFreqId; // We keep the old max
		this->sequence.push_back (freqChunk);
		seqSize = 1;
		assert (freqChunk.freqId < this->nbFreqs);
		acc = 0; // In this case, we don't want to distribute anything to the others
	}
	
	// Redistributes unsignificant time ratios
	if (acc > 0) {
		unsigned int seqSize = this->sequence.size ();
		float timeRatioPerChunk = acc / seqSize;
		float checker = 0.;
		for (unsigned int i = 0; i < seqSize; i++) {
			this->sequence [i].timeRatio += timeRatioPerChunk;
			checker += this->sequence [i].timeRatio;
		}
		assert (checker == 1);
	}
	
	assert (seqSize > 0);

#if 0
	std::cerr << "#" << this->unit.getId () << ", st = " << this->totalsleepWin << " {";
	for (unsigned int i = 0; i < seqSize; i++) {
		std::cerr << "{" << this->sequence[i].freqId << ", " << this->sequence [i].timeRatio << "}, ";	
	}
	std::cerr << "}" << std::endl;
#endif	
}
#endif

Decision NewAdaptiveDecisions::evaluate () {
	switch (this->curEvalState) {
		case EVALUATION_INIT:
			return this->initEvaluation ();
		case FREQUENCY_EVALUATION:
			return this->evaluateFrequency ();
		case STEPS_COMPUTATION:
			this->computeSteps ();
			// No break here on purpose
		case SEQUENCE_COMPUTATION: {
			Decision res = DECISION_ZERO_INITIALIZER;
			this->computeSequence ();
			this->curRuntimeState = SEQUENCE_EXECUTION;
			this->curEvalState = EVALUATION_INIT; // For the next evaluation
			this->timeProfiler.evaluate (SEQUENCE_COMPUTATION);
			return res;
			break;
		}
		default:
			std::cerr << "Error: Unknown Evaluation State "
								<< this->curRuntimeState << " not handled" << std::endl;
			exit (EXIT_FAILURE);
	};	
}

Decision NewAdaptiveDecisions::executeSequence () {
	size_t seqSize = this->sequence.size ();

	if (this->currentSeqChunk < seqSize) {
			Decision res = DECISION_DEFAULT_INITIALIZER;
			res.freqId = this->sequence.at (this->currentSeqChunk).freqId;
			res.sleepWin =	this->sequence.at (this->currentSeqChunk).timeRatio
									*		this->totalsleepWin;
			assert (res.sleepWin <= this->totalsleepWin);
			this->currentSeqChunk++;
			return res;
	} else {
		// Reset the current sequence chunk that will be executed in the execution state
		assert (this->currentSeqChunk > 0);
		this->currentSeqChunk = 0;
	
		// Now, we loop back to the evaluation
		this->curRuntimeState = EVALUATION;

		// Evaluate the execution time
		this->timeProfiler.evaluate (EXECUTION_SLOT);
		
		/*#ifdef REST_LOG
			std::ostringstream str;
			str << std::setw(10) << this->totalsleepWin << " ";
			this->timeProfiler.print (str);
			this->log->logOut (str);
		#endif*/
		Decision res = DECISION_ZERO_INITIALIZER;
		return res;
	}
}

// New
Decision NewAdaptiveDecisions::takeDecision () {
	Decision res;
	
	switch (this->curRuntimeState) {
		case EVALUATION:
			res = this->evaluate ();
			break;
		case SEQUENCE_EXECUTION:
			res = this->executeSequence ();
			break;
		default:
			std::cerr << "Error: Unknown Runtime State " << this->curRuntimeState << " not handled" << std::endl;
			exit (EXIT_FAILURE);
	};
	
	return res;
}

void NewAdaptiveDecisions::readCounters () {
	assert (this->prof != 0);

	// We read only if necessary
	if (this->curRuntimeState == EVALUATION) {
		for (unsigned int i = 0; i < this->nbCpuIds; i++) {
			HWCounters hwc;
			this->prof->read (hwc, i);
		}
	}
}
