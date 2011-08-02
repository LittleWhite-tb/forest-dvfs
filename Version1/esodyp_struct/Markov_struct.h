#ifndef __MARKOV_H__
#define __MARKOV_H__

/*
  This code builds a framework that helps model a program's behaviour. It is programmed
  in such a way that is should seem to the user as being a black-box. Not a lot of functions
  are needed to be able to make it run.

  For any problem of installation or usage, please email me at: beyler@dpt-info.u-strasbg.fr

  Copyright (C) 2006 Beyler Jean Christophe

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//If a pointer is 32 bits on this machine... 
//#define __POINTER_32_BIT__

/* The model's structure is defined in this file
 * The model is calibrated by the depth of the construction (how far back does the historic go),
 * number of consecutive miss-predictions before a flush, the number of elements that are used to construct the graph,
 * the size of a block (used for the allocation scheme), how many elements in advance do we predict
 */

 /*

    Macros decide how the model will react (to a certain limit)

  */

//MAX Depth autorized
#define MAX_DEPTH 20

//Macros used for the set function
#define M_CONSDEPTH  1		//Depth
#define M_ERRMAX 2		//Maximum consecutive miss predictions before a flush
#define M_CONSTTL 4		//Time to live
#define M_PREFDIST 8		//Change the prefetching distance

//For debugging purposes, we can store the first elements
#define MAX_FIRST_ADS 30

//Define DEBUG (adds printing to terminal)
//#define __DEBUG__

//NORMAL_VERB gives more information on what is happening but much less than __DEBUG__
//#define __NORMAL_VERB__
////__OPT_VERIF__ tests the accuracy of the model
//#define __OPT_VERIF__
////If __OPT_VERIF__ is defined, then defining OPT_VERIF_VERB__ will allow
////you to see what has been predicted and what was accessed... it helps to see if there is a problem...
//#define __OPT_VERIF_VERB__
//If we want to prefetch all of the elements from depth 1 to depth prefdist
//#define __PREFETCH_ALL__

//If we want to prefetch or not... Use it to calculate overhead
#define __PREFETCH__
//If we want to create graphs
#define __DRAW_DOT_FILES__

//This Macro decides if we check if the graph is changing, it should limit the number of passes in the construction phase
//Basically, if the macro is worth 10 and no change has been noticed for 10 passes, the construction stops
//#define NO_CHANGE_STOP_CONSTRUCT 100

//This macro defines the maximum of graph nodes accepted (this is just a sentinel)
#define MAX_GRAPH 3000

//HANDLE MEMORY ISSUES
//We do suppose that the memory block is enough (little bit of an optimist...)
#define MARKOV_MEMORY_BLOCK 300000

//used in the REST system to determine if a counter value is considered equal
#define HOWBOUNDEDISEQUAL .15
//used in the REST system to determine if a sleep value (in usleep microseconds) is considered equal
#define HOWLONGISEQUAL 700

//End of Macros

/*
 * We need a graph divided into two parts:
 * Binary tree, to accelerate the research of a specific node
 * Each node in the binary tree has a pointer to a root of the prediction graph
 *
 */

/*Instead of having a single stride as information, we'll do an abstraction using a structure */
typedef struct sGlobalAdd {
	float boundedness;
	u_int32_t duration;//in microseconds
} SGlobalAdd;

//Chained link that will be defined later
struct snodelist;

//Prediction node
typedef struct snode {
	SGlobalAdd info;	//Information of this node

	#ifdef __PREFETCH_ALL__
		SGlobalAdd *predsInfo;	//Prediction	
	#else
		SGlobalAdd predInfo;	//Prediction
	#endif

	//Now we have the info for the sons of this node

	//We have the number of times we used the edge to the most used son (and a pointer to that son)
	int vis_probableNext;
	struct snode *probableNext;	//most probable next node

	//We then have a LRU system between the two last sons that followed this node
	//And of course the number of times we followed their edges
	int vis_last;
	struct snode *last;
	int vis_last2;
	struct snode *last2;

	struct snodelist *next;	//list of the other sons
} SNode;

//Definition of the chained links
typedef struct snodelist {
	SNode* val;
	int visit;		//nbr of visits to this node, useful to know who is the most frequent son
	struct snodelist *next;
} SNodeList;

//For the binary tree
/*
 *  It is used to accelerate the research of a specific node in the prediction graph
 *  Leaves have a next that is worth NULL
 *  The other nodes have a next that is NOT worth NULL
 */
typedef struct snodebin {
	SNode* val;		//Pointer to the associated prediction node
	struct snodebin *left, *right;
} SBinNode;

typedef struct sMarkov {
	int depth;		//Depth of the Markovian model

	int nbr_err;		//nbr of consecutive miss-predictions
	int nbr_err_max;	//Maximum of consecutive miss predictions tolerated before a flush

	SBinNode* root;		//Root of the binary tree

	SNode **cur;		//Pointers on the graph
	int iter;		//Index on the cur table, used in the construct function

	void (*fct) (struct sMarkov *, SGlobalAdd*);	//Function that will be used by the programs that want to be optimized


	int fct_ttlmax;		//Maximum Time to live for the construction function
	int fct_ttl;		//Current time to live for the construction function

	int prefDist;		//Distance of the markovian model (i.e: number of jumps we make in the graph to predict in advance)

	//For the statistics
#ifdef __OPT_VERIF__
	SGlobalAdd pred_made[MAX_DEPTH];
	int last_pred, new_pred;
	//general counters
	int cnt_max_creat_node;	//maximum of nodes created in one pass
	int cnt_creat_node, cnt_dest_node;
	int cnt_reseted;
	int cnt_calls;
	int cnt_bad_pred, cnt_good_pred, cnt_jumps;	//bad predictions, good predictions and number of jumps 
#endif

	//key for the memory management
	void *startpointer, *nextpointer;
	int sizeleft;

#ifdef __DEBUG__
	//first addresses
	int nfirst_ads;
	SGlobalAdd first_ads[MAX_FIRST_ADS];
#endif

#ifdef NO_CHANGE_STOP_CONSTRUCT	
	int no_change_cnt;	//no_change_cnt is to check if we can stop the construction prematurely
#endif

	//number of nodes constructed
	int nbr_nodes;
} SMarkov;


/* generated by genproto */
SMarkov* Markov_Initialize(); 							//Initialize function
void Markov_Reset(SMarkov *m);							//Reset the model
void Markov_Clear(SMarkov *m);							//Clears the model (and frees the memory)
void Markov_Set(SMarkov *m, int v, int mask);					//Set certain attributes

int Markov_OutputGraph(SMarkov *m);						//Output graph to terminal
int Markov_OutputGraphDot(SMarkov *m, const char *fname);				//Output graph to .dot format

void Markov_DisplayBalise(SMarkov *m);
void Markov_DisplayInfo(SMarkov *m);						
void Markov_DisplayInfoSummarized(SMarkov *m);					
void Markov_DisplayInfoReallySummarized(SMarkov *m);

#endif
