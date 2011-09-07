#include "Markov.h"

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

//Update test
static SMarkov* Markov_AllocationInit(void);
static SBinNode* Markov_CreateBinaryTreeNode(SMarkov* m);
static SNode* Markov_CreatePredictionNode(SMarkov* m);
static SNode* Markov_SearchNode(SGlobalAdd* info,SBinNode* n);
static SNode* Markov_SearchOrAddNode(SMarkov* m, SGlobalAdd* info);
static SNode* Markov_AddGraphNode(SMarkov* m,SNode* n, SGlobalAdd* info);
static SGlobalAdd* Markov_FirstCall(SMarkov* m, SGlobalAdd* info);
static SGlobalAdd* Markov_ConstructBegin(SMarkov* m, SGlobalAdd* info);
static SGlobalAdd* Markov_Construct(SMarkov* m, SGlobalAdd* info);
static SGlobalAdd* Markov_ConstructDepth1(SMarkov* m, SGlobalAdd* info);
static void Markov_CalcInfoPred(int pref_dist, SNode* n);

#define Markov_PrefetchItNoReally(x) {return x;} //using a macro to hack the code for REST
//static inline void Markov_PrefetchItNoReally(SGlobalAdd* x);
static SGlobalAdd* Markov_Idle(SMarkov* m, SGlobalAdd* info);
static SGlobalAdd* Markov_Predict(SMarkov* m,SGlobalAdd* info);
static void Markov_UpdateVisits(SNode* n);
static void Markov_OutputPredictionTree(SMarkov* m, SNode* n, int depth);
static void Markov_OutputBinaryTree(SMarkov* m,SBinNode* n);
static void Markov_OutputTemporaryArcsDot(FILE *f, SNode *n);
static void Markov_OutputPredictionTreeDot(FILE* f, SMarkov* m, SNode* n, int depth);
static void Markov_OutputBinaryTreeDot(FILE* f, SMarkov* m,SBinNode* n);
static void * Markov_Malloc(SMarkov* m, size_t size);

#ifdef __OPT_VERIF__
static void Markov_CheckPred(SMarkov* m,SGlobalAdd* info);
static void Markov_CommitPred(SMarkov* m, SGlobalAdd* info);
#endif

#ifdef __DEBUG__
#define DPRINTF(fmt, args...) printf("-DBG- " fmt, ##args)
#else
#define DPRINTF(fmt, args...)
#endif

/*
 * To keep an abstraction and make the code portable we use a structure...
 * Let's define a few functions to reset, set and get, cmp information
 */

static void INFO_print(SGlobalAdd* info)
{
	printf("Boundedness: %f, Duration: %d\n", info->boundedness, info->duration);
}

static void INFO_reset(SGlobalAdd* info)
{
	info->boundedness = -1.0;
	info->duration = 0;
}

static int INFO_isNull(SGlobalAdd* info)
{
	return (info->boundedness==-1.0)&&(info->duration==0);
}

static void INFO_OutputFile(FILE* f,SGlobalAdd* info)
{
	fprintf(f,"Boundedness: %f, Duration: %d", info->boundedness, info->duration);
}

//Works like strcmp, returns 1 if a>b, -1 if a<b else 0
static int INFO_compare(SGlobalAdd* a, SGlobalAdd* b)
{
	u_int32_t boundedA, boundedB;
	u_int32_t durationA, durationB;

//  	DPRINTF("INFO_compare start a: %p, b: %p\n",a,b);

	boundedA = a->boundedness;
	boundedB = b->boundedness;
        durationA = a->duration;
	durationB =  b->duration;

//	DPRINTF("INFO_compare finish\n");

        /* in case strict compare() semantics are needed */
        
	if(abs(boundedA-boundedB) < HOWBOUNDEDISEQUAL) {
		if(abs(durationA-durationB) < HOWLONGISEQUAL)
			return 0;
		else if(durationA<durationB)
			return -1;
		else
			return 1;
	}
	else if(boundedA<boundedB)
		return -1;
	else
		return 1;
	
}


/*
 Base allocation function, creates a huge memory block
*/
static SMarkov* Markov_AllocationInit(void)
{
	void *tmp=malloc((size_t) MARKOV_MEMORY_BLOCK);
	SMarkov* res = (SMarkov*) tmp;

	if(res == NULL) {
		printf("Problem with memory!!!\n");
		exit(1);
	}

	// Set the info to 0
	memset(res,0,sizeof *res);

	res->startpointer = (char*) tmp+sizeof(SMarkov);
	res->nextpointer = res->startpointer;
	res->sizeleft = MARKOV_MEMORY_BLOCK - sizeof(SMarkov);

	return res;
}


/*
 Our own malloc is a little bit of simplification, we have our big memory block,
 just give the current free pointer and update it
 */
static void * Markov_Malloc(SMarkov* m, size_t size)
{
	void *tmp;

	m->sizeleft -= size;
	if(m->sizeleft < 0) {
		printf("No more space for markov!!!\n");
		exit(1);
	}

	tmp = m->nextpointer;
	m->nextpointer = (char*) m->nextpointer + size;

	// Set to 0
	memset(tmp,0,(size_t)size);
	return tmp;
}

//BASE FUNCTIONS OF THE MODEL
SMarkov* Markov_Initialize()	//creation function
{
  	//allocation
	SMarkov* res = Markov_AllocationInit();

#ifdef __DEBUG__
 	res->nfirst_ads = 0;
#endif

	// Default values
#ifdef NO_CHANGE_STOP_CONSTRUCT
	res->no_change_cnt = NO_CHANGE_STOP_CONSTRUCT;
#endif
	// Depth of construction
	res->depth = 2;

	// allocation of the current pointers
	res->cur = Markov_Malloc(res,sizeof(SNode*)*(res->depth));

	/*
	 set iter to -res->depth, this will give the construction
	 function the depth of the construction
         */
	res->iter = -res->depth;

	// # of consecutive mistakes accepted
	res->nbr_err_max = 20;

	// calls to (*fct)() until we change from construction to prediction phase
	res->fct_ttlmax = 30;
	res->fct_ttl = res->fct_ttlmax;

	// Set up first call
	res->fct = &Markov_FirstCall;

	// distance of prefetching
	res->prefDist = 3;

	DPRINTF("Module markov init\n");

#ifdef __OPT_VERIF__
	// this will help the test functions... ONLY used if __OPT_VERIF__ is defined
	res->last_pred = -res->prefDist;
	printf("Verifying predictions\n");
#else
	printf("Not verifying predictions\n");
#endif

#ifdef __PREFETCH__
	printf("Prefetch on\n");
#else
	printf("Prefetch off\n");
#endif
	return res;
}

//Reset function
void Markov_Reset(SMarkov* m)
{
	//if m is allocated (Paranoiac test!)
	if(m) {
#ifdef __NORMAL_VERB__
		printf("Reseting\n");
#endif

		m->root=NULL;

#ifdef __OPT_VERIF__
		m->last_pred = -m->prefDist;
		m->new_pred = 0;

		//Reset nbr of mistakes
		m->nbr_err = 0;

		//Check max created in one construction phase
		if(m->cnt_max_creat_node< m->nbr_nodes)
			m->cnt_max_creat_node = m->nbr_nodes;
		//increment the number of times this function has been called
		m->cnt_reseted++;
#endif

		//Reset allocation block
		m->nextpointer = m->startpointer;
		m->sizeleft = MARKOV_MEMORY_BLOCK - sizeof(SMarkov);

		//Set the fct to first_call and reset the time to live variable
		m->fct = &Markov_FirstCall;
		m->fct_ttl = m->fct_ttlmax;

		//Set m->iter to -m->depth for the construction procedure
		m->iter = -m->depth;

		//allocation of the current pointers
		m->cur = (SNode* *) Markov_Malloc(m,sizeof(SNode*)*(m->depth));

		DPRINTF("Module markov reset\n");

#ifdef __NORMAL_VERB__
		printf("Reset done\n");
#endif

		//reset nbr of nodes
		m->nbr_nodes = 0;
	}
}

//Clear function
void Markov_Clear(SMarkov* m)
{
#ifdef __DRAW_DOT_FILES__
	Markov_OutputGraphDot(m,"Clear.dot");
#endif
	if(m) {
		if(m->startpointer) {
			free((char *) m->startpointer-sizeof(SMarkov));
		}
	}

	DPRINTF("Module markov clear\n");
}

//the set function, helps parameter the model
void Markov_Set(SMarkov* m, int v, int mask)
{
	switch(mask)
	{

	/*
	 change the depth (we SUPPOSE the depth is only changed
	 BEFORE construction has started)
         */
	case M_CONSDEPTH:
		if(m->depth!=v) {
			m->depth = v;

			//change the iter variable
			m->iter = -m->depth;

			/*
			 Okay, if someone really looks deep in the code, you'll notice that there is a little memory leak here (in a certain sense)
			 I suppose this won't be called too many times so instead of trying to make 30 lines of code to save 40 bytes that WILL be freed in the
			 end anyway, I'll just leave this...
			 */
			m->cur = Markov_Malloc(m,sizeof(SNode*)*(m->depth));

			DPRINTF("Module markov Set depth %d\n",m->depth);
		}
		break;

	//Gives the number of maximum miss-predictions allowed before a flush
	case M_ERRMAX:
		m->nbr_err_max = v;
		break;

        //Define the time to live of the construction function
	case M_CONSTTL:
		m->fct_ttl = v;
		m->fct_ttlmax = m->fct_ttl;
		break;

	//Define the disance of prefetching
	case M_PREFDIST:
		m->prefDist= v;

#ifdef __OPT_VERIF__
		//if __OPT_VERIF__ is enabled, we set m->last_pred, it is used in check_pred
		m->last_pred = -m->prefDist;
#endif
		break;
	default:
		break;
	}
}

//SMarkov* related functions
//binary tree node creation
static SBinNode* Markov_CreateBinaryTreeNode(SMarkov* m)
{
	//Allocation
	SBinNode* res = (SBinNode*) Markov_Malloc(m, sizeof(SBinNode));

	DPRINTF("Module markov createBinaryTree, created node: %p\n",res);
	//return the node
	return res;
}

//Creates  a node in the prediction graph
static SNode* Markov_CreatePredictionNode(SMarkov* m)
{
	SNode* res;

	//Allocation
	res  = (SNode*) Markov_Malloc(m,sizeof(SNode));

	DPRINTF("Module markov Create_arbpred\n");

	//default values
	INFO_reset(&res->info);

#ifdef __PREFETCH_ALL__
	{
		int i;
		//Allocation
		res->predsInfo = Markov_Malloc(m,sizeof(SGlobalAdd)*m->prefDist);

		for(i=0;i<m->prefDist;i++) {
			INFO_reset(&res->predsInfo[i]);
		}
	}
#else
	INFO_reset(&res->predInfo);
#endif

	//Make the pointers point on itself, that way it will point to a valid node...
	res->probableNext = res;
	res->last = res;
	res->last2 = res;

#ifdef __OPT_VERIF__
	//Increment the number of created node
	m->cnt_creat_node++;
#endif

	//increment the number of nodes
	(m->nbr_nodes)++;

	//return the node
	return res;
}

//Search function in the binary tree
static SNode* Markov_SearchNode(SGlobalAdd* info,SBinNode* n)
{
	DPRINTF("Module markov search info: %p\n",info);

	//This should never be false...
	while(n!=NULL) {
		//Check if we aren t already at the right node
		if(INFO_compare(&(n->val->info),info)==0) {
			//if so, return the graph node
			return n->val;
		}

		//Otherwise we go left or right, depending on the values of pas and n->val->pas
		if(INFO_compare(info,&(n->val->info))==-1) {
                        //go left
			n = n->left;
		} else {
			n = n->right;
		}
	}

	DPRINTF("Module markov SearchNode end\n");

	//return NULL since we did not find it
	return NULL;
}

//find the node with the info , or add it to the binary tree
static SNode* Markov_SearchOrAddNode(SMarkov* m, SGlobalAdd* info)
{
	SBinNode* tmp;
	DPRINTF("Module markov SearchOrAddNode\n");

	//if we don t have a node yet, this will be the root of the binary tree
	if(m->root==NULL) {
		//create a binary node
		m->root = Markov_CreateBinaryTreeNode(m);
		//create a graph node
		m->root->val = Markov_CreatePredictionNode(m);

		//update the graph node info
		m->root->val->info=*info;

		DPRINTF("Module Markov_SearchOrAddNode first add on\n");

		//return the graph node
		return m->root->val;

	} else {

		//otherwise, we have to see if the node exists in the tree
		tmp = m->root;

		while(1) {
			//DPRINTF("Module markov Markov_SearchOrAddNode\n");

			//if current node info is higher than researched info
			if(INFO_compare(&(tmp->val->info),info) == 1) {

				if(tmp->left) {
					tmp = tmp->left;
				} else {
					//otherwise, we create a node here
					//get a binary node
					tmp->left = Markov_CreateBinaryTreeNode(m);

					//attach a prediction node
					tmp->left->val = Markov_CreatePredictionNode(m);

					//set the information
					tmp->left->val->info = *info;

					DPRINTF("Module markov Markov_SearchOrAddNode, created left son\n");

					return tmp->left->val;
				}

			} else if(INFO_compare(&(tmp->val->info),info) == -1) {
				//otherwise if current node info is inferior to the res. info

				if(tmp->right) {
					tmp = tmp->right;
				} else {
					//otherwise we create a node here
					//get a binary node
					tmp->right = Markov_CreateBinaryTreeNode(m);
					//attach a prediction node
					tmp->right->val = Markov_CreatePredictionNode(m);

					//set the information
					tmp->right->val->info = *info;
					DPRINTF("Module markov Markov_SearchOrAddNode, created right son\n");

					return tmp->right->val;
				}

			} else 	{
				//otherwise we are at the right place so we return the
				//attached node
				DPRINTF("Module markov Markov_SearchOrAddNode, we found the node\n");

				return tmp->val;
			}
		}
	}
}

//add a son that will have the information info to the node n
//if n already has such a node, just return it...
static SNode* Markov_AddGraphNode(SMarkov* m,SNode* n, SGlobalAdd* info)
{
	SNodeList* ltmp,*ltmp2;

	//get the first son
	ltmp = n->next;

	//does n have at least one node?
	if(!ltmp) {
		//create a first son
		//allocation of the edge
		ltmp = Markov_Malloc(m,sizeof(SNodeList));

		//create a node attached to the edge
		ltmp->val = Markov_CreatePredictionNode(m);

		//set the info
		ltmp->val->info = *info;

		//set the edge to the current node
		n->next = ltmp;

		//update variables probableNext, last and last2
		n->probableNext = ltmp->val;
		n->last = ltmp->val;
		n->last2 = ltmp->val;

		//say that we changed something
#ifdef NO_CHANGE_STOP_CONSTRUCT
		m->no_change_cnt = NO_CHANGE_STOP_CONSTRUCT;
#endif

		//return new node
		return ltmp->val;
	}

	//otherwise we DO have a son
	//The list of sons are ordered, so we check if the new info is lower than the first son
	if(INFO_compare(info,&(ltmp->val->info))==-1) {
		//if so, we create a new node that will become the head of the list
		//allocate the edge
		ltmp2 = (SNodeList*) Markov_Malloc(m,sizeof(SNodeList));

		//attach a node to the edge
		ltmp2->val = Markov_CreatePredictionNode(m);

		//set the new info
		ltmp2->val->info = *info;
		//attach the current list of sons to this new one
		ltmp2->next = ltmp;
		//attach the edge to the current node (our new edge is head of the list now)
		n->next = ltmp2;

		//say that we changed something
		#ifdef NO_CHANGE_STOP_CONSTRUCT
			m->no_change_cnt = NO_CHANGE_STOP_CONSTRUCT;
		#endif
		//return new node
		return ltmp2->val;
	} else if(INFO_compare(info,&(ltmp->val->info))==0) {

		//we found the node, we will just return it
		return ltmp->val;

	} else {

		//until we reach the end
		while(ltmp->next) {

			if(INFO_compare(&ltmp->next->val->info,info)==-1) {
				//if k is inferior to the researched info
				//we move to the next node
				ltmp=ltmp->next;

			} else {

				if(INFO_compare(&(ltmp->next->val->info),info)==0) {
					//if we have found it
					return ltmp->next->val;

				} else {
					//otherwise, we create a node here
					//allocate the edge
					ltmp2 = Markov_Malloc(m,sizeof(SNodeList));

					//attach a node to the edge
					ltmp2->val = Markov_CreatePredictionNode(m);

					//set the info to the node
					ltmp2->val->info = *info;

					//set the next nodes of the next edge
					ltmp2->next = ltmp->next;
					//put this new edge into the list
					ltmp->next = ltmp2;
					//say that we changed something
#ifdef NO_CHANGE_STOP_CONSTRUCT
					m->no_change_cnt = NO_CHANGE_STOP_CONSTRUCT;
#endif
					//return the new prediction node
					return ltmp2->val;
				}
			}
		}

		//if we are at this point, this means we at the end of the sons list
		//allocate a new edge
		ltmp2 = Markov_Malloc(m,sizeof(SNodeList));

		//get attached node
		ltmp2->val = Markov_CreatePredictionNode(m);

		//set the info
		ltmp2->val->info = *info;

		//set ltmp2 to NULL since we are at the end of the list
		ltmp2->next = NULL;
		//attach the new edge to the end of the sons list
		ltmp->next = ltmp2;
		//say that we changed something
#ifdef NO_CHANGE_STOP_CONSTRUCT
		m->no_change_cnt = NO_CHANGE_STOP_CONSTRUCT;
#endif

		//return the new node
		return ltmp2->val;
	}
}


//first call, used to put m->adress in place
static SGlobalAdd* Markov_FirstCall(SMarkov* m, SGlobalAdd* info)
{
	DPRINTF("Module markov FirstCall()\n");

	(void) info;

	//next time we call construct
	if(m->depth==1) {
		m->iter = 0;
 		m->fct = &Markov_ConstructDepth1;
	} else {
		m->fct = &Markov_ConstructBegin;
	}

#ifdef __OPT_VERIF__
	m->cnt_calls++;
#endif
	return NULL;
}


static SGlobalAdd* Markov_ConstructBegin(SMarkov* m, SGlobalAdd* info)
{
	//if we are at the beginning of the construction phase, the process is slightly different
	//which explains two construction procedures...
	int i;

#ifdef __OPT_VERIF__
	m->cnt_bad_pred++;
	m->cnt_calls++;
#endif
	DPRINTF("Module markov construct begin with depth:%d Iter:%d\n", m->depth, m->iter);

	/*
	 add a son for each current pointer
	 Reminder: Markov_AddGraphNode adds a son ONLY if the node doesn t already have one
	 We start by not entering the loop
	 The second time, we construct a son for the unique node of the graph at index (m->depth-1)
	 The third time, we construct a son for the first node and a second at the index m->depth-2
	 And so on, until m->iter is >=0
	 Of course, the current pointers move to the new son every call...
	 i.e: cur[0] of the call i will be the father of cur[0] on the call (i+1)
	 */

	for(i=m->depth-1;i>=-m->iter;i--) {
		DPRINTF("Module markov Construct for pointer %d\n",i);
		m->cur[i]=Markov_AddGraphNode(m,m->cur[i],info);
	}

	//we increment iter
	m->iter++;

        /* Sanity check*/
	if(m->iter>0) {
		printf("What the ****???? iter: %d\n",m->iter);
		exit(1);
	}

	m->cur[-m->iter] = Markov_SearchOrAddNode(m,info);

	DPRINTF("Module markov End of construct %d\n",m->iter);

	/*
	 if m->iter is worth 0
	 then this will be the last time we come through here
	 to make the program work correctly, we set it to depth-1
	 To explain this, let us just remind that m->iter represents the node that will overlap
	 In our case, this node is at the index depth-1
	 That is why we set iter to that value.
	 We also change m->fct because it is now the turn of the construct function
	 */

	if(!m->iter) {
		m->iter = m->depth-1;
		m->fct = &Markov_Construct;
	}
	return NULL;
}

//Second phase of the construction of the prediction graph
//We have the Markovian model and the address that has been accessed
static SGlobalAdd* Markov_Construct(SMarkov* m, SGlobalAdd* info)
{
	int i,j;
	SNodeList* ltmp,*ltmp2;

#ifdef __OPT_VERIF__
	m->cnt_calls++;
	m->cnt_bad_pred++;
#endif

	DPRINTF("Module markov Construct\n");
#ifdef __DEBUG__

	//Complete the first_ads table
	if(m->nfirst_ads< MAX_FIRST_ADS) {
		m->first_ads[m->nfirst_ads] = *((SGlobalAdd*) info);
		m->nfirst_ads++;
	}
#endif

	/*
	 m->iter is there to know which is the lastest node created...
	 It is not easy to explain here how it works but I will try...
	 We create sons to the nodes given by the array cur in the SMarkov* structure
	 m->iter represents the node that is at the required depth, its son will now overlap on
	 the (m->iter-1) node.
	 After the creation, we update each current node position to the new (or old) son.
	 The (m->iter-1) node becomes a node that is at the required depth, it is his turn to overlap
	 Hence, we decrement m->iter...
	 Of course, we use a modular and thus we must be careful when m->iter is worth 0...
	 Hope I was a little bit clear...
	 */

	//set j to m->depth, will be needed later...
	j = m->depth;

	DPRINTF("Module markov Construct start of actualisations, m->iter: %d, end: %d\n",m->iter,j);

	/*
	 We are going to add a node that will be labeled pas to each current node
	 We distinguish the node[m->iter-1] from the others
	 We separate the nodes [0,m->iter-1[ and [m->iter+1,m->depth[ so that we do not have the
	 test if (i!=m->iter-1) in the loop
	 */
	for(i=0;i<m->iter-1;i++) {
		m->cur[i] = Markov_AddGraphNode(m,m->cur[i],info);
	}

	DPRINTF("Module markov Construct start of actualisations 2\n");

	/*
	 j will represent the maximum of the second loop
	 if m->iter is worth 0 we must not add a son to the node depth-1 (at least not yet)
	 Remember that j has been set to m->depth...
	 we separated the test j = (m->iter)?m->depth:m->depth-1; because of a glitch with the compiler
	 */
	if(!m->iter) {
		//now j = m->depth - 1
		j--;
	}

	//We start after m->iter
	for(i=m->iter+1;i<j;i++) {
		m->cur[i] = Markov_AddGraphNode(m,m->cur[i],info);
	}

	//We set j to (m->iter-1)%m->depth... (Of course we know 0<= m->iter < m->depth)
	if(m->iter) {
		j = m->iter-1;
	}
	//No need of else, since, in that case, j is already equal to m->depth-1

	//Add a node to this current node
	m->cur[j] = Markov_AddGraphNode(m,m->cur[j],info);

	DPRINTF("Module markov Construct update of m->iter-1: %d and m->iter: %d\n",j,m->iter);

	//It is to the node m->cur[j] that we will overlap the current node [m->iter]
	//We check to see if m->cur[m->iter] has a son
	ltmp = m->cur[m->iter]->next;

	//If not or if the new info is lower than the other sons, we must add the edge at the beginning
	if((!ltmp)||(INFO_compare(info,&(ltmp->val->info))==-1)) {
		//allocate
		ltmp2 =  Markov_Malloc(m,sizeof(SNodeList));

		//we link the other sons (if any) to this edge
		ltmp2->next = ltmp;

		//we attach the new list of sons to the current node
		m->cur[m->iter]->next = ltmp2;

		//we attach the new edge to the node m->cur[j], enabling the overlap
		ltmp2->val = m->cur[j];

		//We update m->cur[m->iter]->probableNext, last and last2
		//All that matters is that they start off with a son that is valid...
		//Note that these variables were initialized to point on their own structure,
		//That way, they ALWAYS point to a valid structure...

		m->cur[m->iter]->probableNext = ltmp2->val;
		m->cur[m->iter]->last = ltmp2->val;
		m->cur[m->iter]->last2 = ltmp2->val;

		//say that we changed something
#ifdef NO_CHANGE_STOP_CONSTRUCT
		m->no_change_cnt = NO_CHANGE_STOP_CONSTRUCT;
#endif

	} else {

		//Otherwise we have to find the correct place where to insert (if needed) the new son
		while((ltmp->next)&&(INFO_compare(&(ltmp->next->val->info),info)==-1)) {
			ltmp=ltmp->next;
		}

		//If we are at the end of the list or if ltmp->next->val->info>info then we have to create a new son
		//The test ltmp->val->info!=info checks the special case where the the current node has only one son
		//that is labeled with the correct info
		if(
		   (INFO_compare(&(ltmp->val->info),info)) &&
		   ((!ltmp->next)||(INFO_compare(&(ltmp->next->val->info),info)==1) )
		  )
		{
			//we create an edge that will link us to the node m->iter-1
			ltmp2 = Markov_Malloc(m,sizeof(SNodeList));

			//link this new edge to the list
			ltmp2->next = ltmp->next;
			ltmp->next = ltmp2;

			//attach the node to this edge, enabling the overlap
			ltmp2->val = m->cur[j];
			//say that we changed something
#ifdef NO_CHANGE_STOP_CONSTRUCT
			m->no_change_cnt = NO_CHANGE_STOP_CONSTRUCT;
#endif
		}

		//otherwise we already have a son with that info, nothing has to be done
	}

	/*
	 now the m->cur[m->iter] node will be reseted and will point to the root node that has the label info
	 We use the binary tree to find the root node and the function SearchorAddNode that
	 Returns the node if it is found or returns a newly created (and linked) one
	*/
	m->cur[m->iter] = Markov_SearchOrAddNode(m,info);

	/*
	 we decrement m->iter (being careful of the modular)
	 because of a glitch in the compiler, we separated the test in two parts
	 normally this is the instruction: m->iter = (m->iter)?m->iter--:m->depth-1;
	 this is the first part, the second is later in the code...
	 We start by decrementing the value and then check if it's negative...
	 */
	m->iter--;

	//we decrement the time to live variable
	m->fct_ttl--;


#ifdef NO_CHANGE_STOP_CONSTRUCT
	//decrement no_change_cnt
	(m->no_change_cnt)--;

	if(!(m->no_change_cnt)) {
		//printf("We made: %d construction passes\n",m->fct_ttlmax-m->fct_ttl);
		m->fct_ttl = 0;
	}
#endif

	//second part of the m->iter = (m->iter)?m->iter--:m->depth-1;
	if(m->iter<0) {
		m->iter = m->depth-1;
	}

#ifdef MAX_GRAPH
	//increment the number of nodes
	if(m->nbr_nodes>MAX_GRAPH) {
		printf("MAX_GRAPH\n");
		//just put the time to live at 0
		Markov_Reset(m);
		m->fct = &Markov_Idle;
		m->fct_ttl=0;

		return NULL;
	}
#endif


	// if fct_ttl is 0 we change from construction to prediction phase
	if(m->fct_ttl==0) {
#ifdef __DRAW_DOT_FILES__
		Markov_OutputGraphDot(m,"GraphEndCons.dot");
#endif

		/*
		 1) reset ttl
		 2) change the (*fct)() pointer to the prediction function
		 3) we set m->cur[0] to the last pointer m->cur[m->iter]
		 This is done because predict will only use m->cur[0]
		 */
		m->fct_ttl = m->fct_ttlmax;
		m->fct = &Markov_Predict;
		m->cur[0] = m->cur[m->iter];

#ifdef __OPT_VERIF__
		DPRINTF("Construction finished\n");
		DPRINTF("Created %d\n",m->cnt_creat_node);
		DPRINTF("Current node %p\n",m->cur[0]);
#ifdef __DEBUG__
		Markov_DisplayInfo(m);
#endif

#endif
	}
return NULL;
}

//Special case for construction depth of 1
static SGlobalAdd* Markov_ConstructDepth1(SMarkov* m, SGlobalAdd* info)
{
	SNode* ntmp;
	SNodeList* ltmp, *ltmp2;

#ifdef __OPT_VERIF__
	m->cnt_calls++;
	m->cnt_bad_pred++;
#endif


	DPRINTF("Module markov Construct Depth 1\n");
#ifdef __DEBUG__
	//Complete the first_ads table
	if(m->nfirst_ads< MAX_FIRST_ADS) {
		m->first_ads[m->nfirst_ads] = *((SGlobalAdd*) (info));
		m->nfirst_ads++;
	}
#endif

	//special case
	if(!m->cur[0]) {
		m->cur[0] = Markov_SearchOrAddNode(m,info);
		return NULL;
	}

	//This version is simple just use the rech_ou_ajoute function
	ntmp = Markov_SearchOrAddNode(m,info);

	//It is to the node m->cur[0] that we will overlap the current node
	//We check to see if m->cur[0] has a son
	ltmp = m->cur[0]->next;

	//If not or if the new info is lower than the other sons, we must add the edge at the beginning
	if((!ltmp)||(INFO_compare(info,&(ltmp->val->info))==-1)) {

		//allocate
		ltmp2 = Markov_Malloc(m,sizeof(SNodeList));

		//we link the other sons (if any) to this edge
		ltmp2->next = m->cur[0]->next;

		//we attach the new list of sons to the current node
		m->cur[0]->next = ltmp2;

		//we attach the new edge to the node m->cur[j], enabling the overlap
		ltmp2->val = ntmp;

		/*
		 We update m->cur[0]->probableNext, last and last2
		 All that matters is that they start off with a son that is valid...
		 Note that these variables were initialized to point on their own structure,
		 That way, they ALWAYS point to a valid structure...
		 */

		m->cur[0]->probableNext = ltmp2->val;
		m->cur[0]->last = ltmp2->val;
		m->cur[0]->last2 = ltmp2->val;

		//say that we changed something
#ifdef NO_CHANGE_STOP_CONSTRUCT
		m->no_change_cnt = NO_CHANGE_STOP_CONSTRUCT;
#endif
	} else {

		//Otherwise we have to find the correct place where to insert (if needed) the new son
		while((ltmp->next)&&(INFO_compare(&(ltmp->next->val->info),info)==-1))
			ltmp=ltmp->next;

		//If we are at the end of the list or if ltmp->next->val->info>info then we have to create a new son
		//The test ltmp->val->info!=info checks the special case where the the current node has only one son
		//that is labeled with the correct info
		if(
		   (INFO_compare(&(ltmp->val->info),info)) &&
		   ( (!ltmp->next) || (INFO_compare(&(ltmp->next->val->info),info)==1) )
		  )
		{
			//we create an edge that will link us to the node m->iter-1
			ltmp2 = Markov_Malloc(m,sizeof(SNodeList));

			if(ltmp2==NULL) {
				//we do not really take care of this problem yet...
				//in general, it should not happen
				printf("Allocate problem 2 in construct\n");
				return NULL;
			}

			//link this new edge to the list
			ltmp2->next = ltmp->next;
			ltmp->next = ltmp2;

			//attach the node to this edge, enabling the overlap
			ltmp2->val = ntmp;

			//say that we changed something
#ifdef NO_CHANGE_STOP_CONSTRUCT
			m->no_change_cnt = NO_CHANGE_STOP_CONSTRUCT;
#endif
		}

		//otherwise we already have a son with that info, nothing has to be done
	}

#ifdef NO_CHANGE_STOP_CONSTRUCT
	//now check if a change has been made
	if(tmp!=m->nbr_nodes) {
		m->no_change_cnt = NO_CHANGE_STOP_CONSTRUCT;
	}
#endif

	//set m->cur[0]
	m->cur[0] = ntmp;

	//we decrement the time to live variable
	m->fct_ttl--;

#ifdef NO_CHANGE_STOP_CONSTRUCT
	//decrement no_change_cnt
	(m->no_change_cnt)--;

	if(!(m->no_change_cnt)) {
		DPRINTF("We made: %d construction passes\n",m->fct_ttlmax-m->fct_ttl);
		m->fct_ttl = 0;
	}
#endif

#ifdef MAX_GRAPH
	//increment the number of nodes
	if(m->nbr_nodes>MAX_GRAPH) {
		printf("MAX_GRAPH\n");
		//just put the time to live at 0
		Markov_Reset(m);
		m->fct = &Markov_Idle;
		m->fct_ttl=0;
		return NULL;
	}
#endif

	//if it is equal to zero
	if(m->fct_ttl==0) {
#ifdef __DRAW_DOT_FILES__
		Markov_OutputGraphDot(m,"GraphEndCons.dot");
#endif

		/*
		 1) reset TTL
		 2) change from construction to prediction phase
                 */
		m->fct_ttl = m->fct_ttlmax;
		m->fct = &Markov_Predict;
		//This is done because predict will only use m->cur[0]
		DPRINTF("Construction finished, created\n");
	}
	return NULL;
}

//calculates the address to be fetched, using the value of pref_dist to determine the how far in the future do we go
static void Markov_CalcInfoPred(int pref_dist, SNode* n)
{
	SNode* tmp = n->probableNext;

#ifdef __PREFETCH_ALL__
	int i=0;
#endif

	//while we still have a son or that the distance is not yet reached
	//Little information: If ever tmp==NULL before pref_dist==0, we'll be prefeching what INFO_reset fills up...
	while(pref_dist && tmp) {
#ifdef __PREFETCH_ALL__
		//set info
		n->predsInfo[i] = tmp->info;
		i++;
#else
		//set info
		n->predInfo = tmp->info;
#endif
		//go to the next node
		tmp = tmp->probableNext;
		//decrement the distance
		pref_dist--;
	}

#ifdef __PREFETCH_ALL__
	while(pref_dist) {
		INFO_reset(&(n->predsInfo[i]));
		i++;
		pref_dist--;
	}
#endif
}

//inline prefetch function
/* commented out for REST... replaced by macro at top of file

static inline void Markov_PrefetchItNoReally(SGlobalAdd* x)
{*/

	//only lfetch
	/*
	 __asm__ __volatile__ (
	 "lfetch.nta [%0]\n"
	 //"ld1.s r31=[%0]\n"
	 ::"r" (x)
	 );
	 */
//}

/*
 Verification functions

 These counting functions suppose a prefech will be made almost every call to
 Markov_Predict... -> Markov_CommitPred

 If not, the numbers will be unaccurate but it is a little price to pay otherwise
 maintaining the list of elements prefeched became a hassle.

 We also suppose only one prefetch is made per call to Markov_predict...

*/

#ifdef __OPT_VERIF__
static void Markov_CheckPred(SMarkov* m,SGlobalAdd* info)
{
	//we have a prediction only if last_pred is >= 0
	if(m->last_pred >= 0) {
#ifdef __OPT_VERIF_VERB__
		printf(
		       "We have: %p \t Result: %d\n",
		       &m->pred_made[m->last_pred],
		       INFO_compare(&(m->pred_made[m->last_pred]),info)==0
		      );
#endif

		//if it is the same address, our prediction was correct
		if(INFO_compare(&(m->pred_made[m->last_pred]),info) == 0) {
			//increment the correct predictions
			m->cnt_good_pred++;
		} else {
			m->cnt_bad_pred++;
		}
	} else {
		m->cnt_bad_pred++;
	}

	//increment new_pred
	m->new_pred = (m->last_pred<0) ? m->new_pred++ : (m->last_pred+m->prefDist)%MAX_DEPTH;

	//we increment last_prediction being careful with the modular
	m->last_pred = (m->last_pred<0) ? m->last_pred+1 : (m->last_pred+1)%MAX_DEPTH;
}

//check pred puts the jump that was made in the table to be checked by check_jump
static void Markov_CommitPred(SMarkov* m, SGlobalAdd* info)
{
#ifdef __OPT_VERIF_VERB__
	static int cnt =0;

	printf("\tCurrent node: %p\n",(void *) m->cur[0]);
	if(m->cur[0]) {
		printf(
		       "\tprobableNext: %p, last: %p, last2: %p\n",
		       m->cur[0]->probableNext,
		       m->cur[0]->last,
		       m->cur[0]->last2
		      );
		printf(
		       "\tVis_probableNext: %d, Vis_last: %d, Vis_last2: %d\n",
		       m->cur[0]->vis_probableNext,
		       m->cur[0]->vis_last,
		       m->cur[0]->vis_last2
		      );
	}
#endif

	//put the adress in the table at the index m->new_pred
	m->pred_made[m->new_pred] =  *((SGlobalAdd*) info);


#ifdef __OPT_VERIF_VERB__
	cnt++;
	if(cnt > 500) {
		exit(0);
	}
#endif
}
#endif

static SGlobalAdd* Markov_Idle(SMarkov* m, SGlobalAdd* info)
{
	//idle function...
	(void) m;
	(void) info;
	return NULL;
}


//predict is the function for the prediction phase
static SGlobalAdd* Markov_Predict(SMarkov* m,SGlobalAdd* info)
{
	SNode* tmp ,*tmp2;
	SNodeList* ltmp;
	
	int max_vis;
	int itmp;
	
	(void) m; 
	(void) info;

	//Check our accuracy
#ifdef __OPT_VERIF__
	m->cnt_calls++;
	Markov_CheckPred(m,info);
#endif

	DPRINTF("Module markov Predict\n");

	//Our position in the graph of prediction is always stored in m->cur[0]
	tmp = m->cur[0];

	//if tmp is NULL, we see if there is not a root node with the correct info
	if(tmp==NULL) {
		//search for a node labeled info in the binary tree (Markov_SearchNode returns the associated prediction node)
		m->cur[0] = Markov_SearchNode(info,m->root);
		//increment the number of consecutive mistakes
  		m->nbr_err++;
		//if this number exceeds a threshold, we flush the graph and restart
		if(m->nbr_err>m->nbr_err_max) {
#ifdef __NORMAL_VERB__
			printf("A flush\n");
#endif
			Markov_Reset(m);
			return NULL;
		}

	} else {
		//Otherwise tmp is not NULL, let us see if we have a son with the info saut

		/*
		 Each node has three special pointers:
		 probableNext: that is the most often followed son
		 last and last2: which are the two last nodes followed

		 And three integers:
		 vis_probableNext, vis_last and vis_last2
		 These represent the number of times we followed the associated edges.
		 If ever, one of these nodes disappears, we update the associated arc
		 */

		//get tmp->probableNext and vis_probableNext
		tmp2 = tmp->probableNext;
		max_vis = tmp->vis_probableNext;

		//If it is the most often followed node
		if(INFO_compare(&(tmp2->info),info) == 0) {
			//We update m->cur[0], it is now worth the most followed son of the current node
			m->cur[0] = tmp2;

			//we reset the number of mistakes
			m->nbr_err = 0;

			//Update visit
			tmp->vis_probableNext++;

			#ifdef __PREFETCH_ALL__
			//If we have not yet calculated the predicted addresses of this node
			if(INFO_isNull(&(tmp2->predsInfo[0]))) {
				Markov_CalcInfoPred(m->prefDist,tmp2);
			}

				#ifdef __PREFETCH__
				{
					int i;
					for(i=0;i<m->prefDist;i++) {
						if(!INFO_isNull(&(tmp2->predsInfo[i]))) {
							Markov_PrefetchItNoReally(&(tmp2->predsInfo[i]));
						}
					}
				}
				#endif
			#else
				//if we have not yet calculated the predicted address of this node
				if(INFO_isNull(&(tmp2->predInfo))) {
					Markov_CalcInfoPred(m->prefDist,tmp2);
				}

				#ifdef __OPT_VERIF__
					//If we want to check the accuracy of the model
					Markov_CommitPred(m,&tmp2->predInfo);
				#endif

				#ifdef __PREFETCH__
					//we prefetch the info
					Markov_PrefetchItNoReally(&tmp2->predInfo);
				#endif
			#endif
			return NULL;
		}

		//if we are here then it is not the most followed edge,
		//perhaps it is the last node followed?
		tmp2 = tmp->last;

		if(INFO_compare(&(tmp2->info),info) == 0) {
			//if so, the current node becomes this one
			m->cur[0] = tmp2;

			//Update visit
			tmp->vis_last++;

			if(tmp->vis_last>max_vis) {
				tmp->last = tmp->probableNext;
				//tmp2 is equal to tmp->last
				tmp->probableNext = tmp2;

				//let us switch the vis_probableNext and vis_last variable
				tmp->vis_probableNext = tmp->vis_last;
				tmp->vis_last = max_vis;

				//recalculate the prediction
				Markov_CalcInfoPred(m->prefDist,tmp);
			} 

			#ifdef __PREFETCH_ALL__
				//If we have not yet calculated the predicted addresses of this node
				if(INFO_isNull(&(tmp2->predsInfo[0]))) {
					Markov_CalcInfoPred(m->prefDist,tmp2);
				}
			#else
				//if we have not yet calculated the prediction of this node
				if(INFO_isNull(&tmp2->predInfo)) {
					Markov_CalcInfoPred(m->prefDist,tmp2);
				}

				#ifdef __OPT_VERIF__
					//If we want to check the accuracy of the model
					Markov_CommitPred(m,&tmp2->predInfo);
				#endif
			#endif

			//we increment the number of mistakes
			m->nbr_err++;
			//if nbr_err is over the threshold
			if(m->nbr_err>m->nbr_err_max) {
				#ifdef __NORMAL_VERB__
					printf("Flushing\n");
				#endif
				Markov_Reset(m);
				return NULL;
			}

			#ifdef __PREFETCH__
				#ifdef __PREFETCH_ALL__
					{
						int i;
						for(i=0;i<m->prefDist;i++) {
							if(!INFO_isNull(&(tmp2->predsInfo[i]))) {
								Markov_PrefetchItNoReally(&(tmp2->predsInfo[i]));
							}
						}
					}
				#else
					//we predict
					Markov_PrefetchItNoReally(&tmp2->predInfo);
				#endif
			#endif
			return NULL;
		}

		//if we still have not found the son than we try with this one
		tmp2 = tmp->last2;

		//Check if there is an equality
		if(INFO_compare(&(tmp2->info),info)==0) {
			//we update m->cur[0]
			m->cur[0] = tmp2;

			//we increment the number of visits made to this node
			tmp->vis_last2++;

			//if ever vis_last2 is bigger than the threshold given by the most probable node
			//we switch
			if(tmp2->vis_last2>max_vis) {
				//tmp2 is worth tmp->last2
				tmp->last2 = tmp->last;
				tmp->last = tmp->probableNext;
				tmp->probableNext = tmp2;

				//Let us switch the vis_probableNext, vis_last and vis_last2 variables
				tmp->vis_probableNext = tmp->vis_last2;
				tmp->vis_last2 = tmp->vis_last;
				tmp->vis_last = max_vis;

				//recalculate the info
				Markov_CalcInfoPred(m->prefDist,tmp);

			}
			#ifdef __PREFETCH_ALL__
				//If we have not yet calculated the predicted addresses of this node
				if(INFO_isNull(&(tmp2->predsInfo[0]))) {
					Markov_CalcInfoPred(m->prefDist,tmp2);
				}
			#else
				//if we have not yet calculated the predicted info of this node
				if(INFO_isNull(&tmp2->predInfo)) {
					Markov_CalcInfoPred(m->prefDist,tmp2);
				}
				#ifdef __OPT_VERIF__
					//If we want to check the accuracy of the model
					Markov_CommitPred(m,&tmp2->predInfo);
				#endif
			#endif

			//increment the number of consecutive mistakes
			m->nbr_err++;
			//if nbr_err is over the threshold
			if(m->nbr_err>m->nbr_err_max) {
				#ifdef __NORMAL_VERB__
					printf("A Flush\n");
				#endif
				Markov_Reset(m);
				return NULL;
			}

			//we predict
			#ifdef __PREFETCH__
				#ifdef __PREFETCH_ALL__
				{
					int i;
					for(i=0;i<m->prefDist;i++) {
						if(!INFO_isNull(&(tmp2->predsInfo[i]))) {
							Markov_PrefetchItNoReally(&(tmp2->predsInfo[i]));
						}
					}
				}
				#else
					Markov_PrefetchItNoReally(&tmp2->predInfo);
				#endif
			#endif
			return NULL;
		}

		//We check the other sons
		ltmp = tmp->next;

		while(ltmp != NULL) {
			#ifdef __OPT_VERIF__
				//if we want more information
				m->cnt_jumps++;
			#endif

			//if the info of this node is higher or equal we break
			if(INFO_compare(&(ltmp->val->info),info)>=0) {
				break;
			}
			ltmp = ltmp->next;
		}

		//if we are at the end or if it is not the right info
		if( (!ltmp) || (INFO_compare(&(ltmp->val->info),info)) ) {
			//we increment the number of mistakes
			m->nbr_err++;
			if(m->nbr_err>m->nbr_err_max) {
				#ifdef __NORMAL_VERB__
					printf("Flushing\n");
				#endif
				Markov_Reset(m);
				return NULL;
			}

		//we try to find a root node with the same label
   		m->cur[0] = Markov_SearchNode(info,m->root);

		//if we do find one, we will use tmp->last to link us to it
		if(m->cur[0]) {

			//before losing vis_last2, we try to see if last2 is a son of this node
			ltmp = tmp->next;
			while(ltmp!=NULL) {
				if(ltmp->val==tmp->last2) {
					//if we have found it
					ltmp->visit = tmp->vis_last2;
					break;
				}

			//Next edge
			ltmp = ltmp->next;
			}

			//we change the pointers
			tmp->last2 = tmp->last;
			tmp->last = m->cur[0];
			//we change vis_last2
			tmp->vis_last2 = tmp->vis_last;

			/*
			 We do not change tmp->vis_last because if we reset it to 0 we might
			 be much lower than tmp->vis_probableNext and so we would jeopardize
			 this node becoming the most followed one...

			 Leaving it, gives us the chance to be in the same ball park than
			 the vis_last2 and vis_probableNext
			 */
			}
		} else {
			//Otherwise we have a son with the right info

			tmp2 = ltmp->val;
			//we update the current pointer
			m->cur[0] = tmp2;

			//we update the vis_last2 and vis_last variables
			itmp = tmp->vis_last2;
			tmp->vis_last2 = tmp->vis_last;
			tmp->vis_last =ltmp->visit;

			//before losing itmp, we check to see if tmp->last2 is a son of this node
			ltmp = tmp->next;
			while(ltmp!=NULL) {
				if(ltmp->val==tmp->last2) {
					ltmp->visit = itmp;
					break;
				}
				ltmp = ltmp->next;
			}

			//update the pointers
			tmp->last2 = tmp->last;
			tmp->last = tmp2;


			#ifdef __PREFETCH_ALL__
				//If we have not yet calculated the predicted addresses of this node
				if(INFO_isNull(&(tmp2->predsInfo[0]))) {
					Markov_CalcInfoPred(m->prefDist,tmp2);
				}
			#else
				//if we have not yet calculated the info of this node
				if(INFO_isNull(&tmp2->predInfo)) {
					Markov_CalcInfoPred(m->prefDist,tmp2);
				}
				#ifdef __OPT_VERIF__
					//If we want to check the accuracy of the model
					Markov_CommitPred(m,&tmp2->predInfo);
				#endif
			#endif


			//prefetch the info
			#ifdef __PREFETCH__
				#ifdef __PREFETCH_ALL__
				{
					int i;
					for(i=0;i<m->prefDist;i++) {
						if(!INFO_isNull(&(tmp2->predsInfo[i]))) {
							Markov_PrefetchItNoReally(&(tmp2->predsInfo[i]));
						}
					}
				}
				#else
					Markov_PrefetchItNoReally(&tmp2->predInfo);
				#endif
			#endif
		}
	}
return NULL;
}

/*
 give us pref_dist predictions
 SNode is usually the CURRENT pointer in the model, but may be any node
 return how many real predictions we have
 */
inline int Markov_PredictN(SGlobalAdd *pref_info, SNode* n, int pref_dist)
{
	int i=0;
	SNode *tmp=n->probableNext;

	/* retain a start pointer so we'll detect a circle */
	SNode *start=n->probableNext;
	
	// while we still have a son or that the distance is not yet reached
	while(pref_dist && tmp) {

		// set info
		pref_info[i]=tmp->info;
		i++;

		// go to the next node
		tmp = tmp->probableNext;
		pref_dist--;
		
		/* abort when we reach the first node again == circle */
		if(tmp == start) {
			fprintf(stderr, "predictN: circle after %d\n", i);
			break;
		}
	}

	// return how many real predictions we have
	return i;
}


//Updates the visits
static void Markov_UpdateVisits(SNode* n)
{
	SNodeList* l;

	//We'll go through the next nodes and check probable, last and last2 node
	l = n->next;

	while(l) {
		if(n->probableNext == l->val) {
			l->visit = n->vis_probableNext;

		} else if(n->last == l->val) {
			l->visit = n->vis_last;

		} else if(n->last2 == l->val) {
			l->visit = n->vis_last2;
		}

		l = l->next;
	}
}

//Display the prediction tree
//depth is there to make sure we do not do an infinite loop
static void Markov_OutputPredictionTree(SMarkov* m, SNode* n, int depth)
{
	int i;
	SNodeList* l;
	//if  is equal to 0 then we exit
	if(!depth) {
		return;
	}

	//otherwise we tab a little
	i = m->depth - depth;

	while(i-- >= 0) {
		printf("\t");
	}

	//then we printf
	INFO_print(&(n->info));

	//Display the sons
	l = n->next;
	while(l) {
		//recursive call, depth decremented
		Markov_OutputPredictionTree(m,l->val,depth-1);
		//next son
		l=l->next;
	}
}

//Display the binary tree
static void Markov_OutputBinaryTree(SMarkov* m,SBinNode* n)
{
	if(!n) {
		return;
	}

	printf("Binary node: ");
	INFO_print(&(n->val->info));

	Markov_OutputPredictionTree(m,n->val,m->depth+1);
	//Display left son
	Markov_OutputBinaryTree(m,n->left);
	//Display right node
	Markov_OutputBinaryTree(m,n->right);

	//Display the prediction graph, we give m->depth + 1 so that we can see the overlapping
}

//Output SMarkov* Model to terminal
int Markov_OutputGraph(SMarkov* m)
{
	if(m==NULL) {
		return EXIT_FAILURE;
	}

	Markov_OutputBinaryTree(m,m->root);
	return EXIT_SUCCESS;
}

void Markov_DisplayBalise(SMarkov* m)
{
        printf("INFOMARKOV %d %d %d %d",m->depth,m->prefDist,m->fct_ttlmax,m->nbr_err_max);
        printf(" %d",m->sizeleft);

#ifdef __OPT_VERIF__    
        printf(" %d %d %d",m->cnt_creat_node, m->cnt_max_creat_node, m->nbr_nodes);
        printf(" %d",m->cnt_reseted);
        printf(" %d %d %d %d",m->cnt_good_pred,m->cnt_pretty_good_pred,m->cnt_bad_pred,m->cnt_jumps);
#endif
        printf("\n\n");
}

//Display the information on the Markovian model
void Markov_DisplayInfo(SMarkov* m)
{
	int i;
	printf("Info on the Markovian model\n");

	printf("Depth: %d\n",m->depth);
	printf("The current pointers are:\n");

	for(i=0;i<m->depth;i++) {
		printf("Pointer %d: %ld\n",i,(long) m->cur[i]);
	}

	//Display the tree and the prediction graph
	Markov_OutputBinaryTree(m,m->root);

#ifdef __OPT_VERIF__
	printf("Nbr of errors: %d\tNbr of maximum errors: %d\n",m->nbr_err,m->nbr_err_max);
	printf("Nodes: %d %d %d\n",m->cnt_creat_node, m->cnt_max_creat_node, m->nbr_nodes);
	printf("Nodes: %d %d %d\n",m->cnt_creat_node, m->cnt_max_creat_node, m->nbr_nodes);
#endif
}

void Markov_DisplayInfoSummarized(SMarkov* m)
{
	printf("Info on the Markovian model\n");
	printf("Depth: %d\ndepth prech: %d\n",m->depth,m->prefDist);

#ifdef __DEBUG__
	{
		int i;

		printf("Nbr of first addresses: %d\n",m->nfirst_ads);
		for(i=0; i<m->nfirst_ads; i++) {
			INFO_print(&m->first_ads[i]);
		}
		printf("\n");
	}
#endif

#ifdef __OPT_VERIF__
	printf("Verifying prefetcher:\n");
	printf("Nodes created: %d\nMaximum of nodes created in one pass: %d\nTotal nbr of nodes: %d\n",m->cnt_creat_node, m->cnt_max_creat_node, m->nbr_nodes);
	printf("Nbr of resets: %d\nNbr of calls: %d\n", m->cnt_reseted,m->cnt_calls);
	printf("Nbr of errors: %d, nbr of maximum errors %d\n",m->nbr_err, m->nbr_err_max);
	printf("Predictions:\nGood:\t%d\nBad:\t%d\nJumps:\t%d\n", m->cnt_good_pred,m->cnt_bad_pred,m->cnt_jumps);
#endif

	Markov_OutputBinaryTree(m,m->root);
}

void Markov_DisplayInfoReallySummarized(SMarkov* m)
{
	printf("Base info: %d %d %d %d\n",m->depth,m->prefDist,m->fct_ttlmax,m->nbr_err_max);
	printf("Space left: %d\n",m->sizeleft);
#ifdef __OPT_VERIF__
	printf("Nodes: %d %d %d\n",m->cnt_creat_node, m->cnt_max_creat_node, m->nbr_nodes);
	printf("Resets: %d\n",m->cnt_reseted);
	printf("Cnts: %d %d %d\n",m->cnt_good_pred,m->cnt_bad_pred,m->cnt_jumps);
#endif
	printf("\n\n");
}


//DOT FUNCTIONS
//Output temporary arcs
void Markov_OutputTemporaryArcsDot(FILE *f, SNode *n)
{
 	int pbn=0, last=0, last2=0;
	SNodeList *l = n->next;

	//We go through the sons and check if it's one of the sons
	while(l) {
		if(l->val == n->probableNext) {
			pbn = 1;
		}

		if(l->val == n->last) {
			last = 1;
		}

		if(l->val == n->last2) {
			last2 = 1;
		}

		l = l->next;
	}

	//It's a temporary node
	if( (!pbn) && (n->vis_probableNext) ) {
#ifdef __POINTER_32_BIT__
		fprintf(f,"%u -> %u",(unsigned int) n, (unsigned int) n->probableNext);
#else
		fprintf(f,"%lu -> %lu",(unsigned long) n, (unsigned long) n->probableNext);
#endif

		fprintf(f," [fontcolor=blue,style=dotted,label=\"%d\"]\n",n->vis_probableNext);
	}

	//It's a temporary node
	if( (!last) && (n->vis_last) ) {
#ifdef __POINTER_32_BIT__
		fprintf(f,"%u -> %u",(unsigned int) n, (unsigned int) n->last);
#else
		fprintf(f,"%lu -> %lu",(unsigned long) n, (unsigned long) n->last);
#endif

  	        fprintf(f," [fontcolor=blue,style=dotted,label=\"%d\"]\n",n->vis_last);
	}

	//It's a temporary node
	if( (!last2) && (n->vis_last2) ) {
#ifdef __POINTER_32_BIT__
		fprintf(f,"%u -> %u",(unsigned int) n, (unsigned int) n->last2);
#else
		fprintf(f,"%lu -> %lu",(unsigned long) n, (unsigned long) n->last2);
#endif

		fprintf(f," [fontcolor=blue,style=dotted,label=\"%d\"]\n",n->vis_last2);
	}

}


//Display the prediction tree
//depth is there to make sure we do not do an infinite loop
static void Markov_OutputPredictionTreeDot(FILE* f, SMarkov* m, SNode* n, int depth)
{
	int i;

	SNodeList* l;
	//if depth is equal to 0 then we exit
	if(!depth) {
		return;
	}

	//i will help for the label...
	i = m->depth - depth;

	//First define label
#ifdef __POINTER_32_BIT__
	fprintf(f,"%u [label=\"(%d)\\n",(unsigned int) n,m->depth-depth);
#else
	fprintf(f,"%lu [label=\"(%d)\\n",(unsigned long) n,m->depth-depth);
#endif

	INFO_OutputFile(f,&(n->info));

	if(depth!=m->depth) {
		fprintf(f,"\"]\n");
	} else {
		fprintf(f,"\",shape=box,color=red,style=filled]\n");
	}

	//Update visits
        Markov_UpdateVisits(n);
	//Display the sons
	l = n->next;
	while(l) {
#ifdef __POINTER_32_BIT__
		fprintf(f,"%u -> %u",(unsigned int) n, (unsigned int) l->val);
#else
		fprintf(f,"%lu -> %lu",(unsigned long) n, (unsigned long) l->val);
#endif

		fprintf(f," [fontcolor=blue,label =\"%d\"]\n",l->visit);
		//recursive call, depth decremented
		Markov_OutputPredictionTreeDot(f,m,l->val,depth-1);
		//next son
		l=l->next;
	}

	Markov_OutputTemporaryArcsDot(f,n);
}

//Display the binary tree
static void Markov_OutputBinaryTreeDot(FILE* f, SMarkov* m,SBinNode* n)
{
	if(!n) {
		return;
	}

	Markov_OutputPredictionTreeDot(f,m,n->val,m->depth);
	//Display left son
	Markov_OutputBinaryTreeDot(f,m,n->left);
	//Display right node
	Markov_OutputBinaryTreeDot(f,m,n->right);
}


int Markov_OutputGraphDot(SMarkov* m, const char *fname)
{
	FILE *f;

	if(m==NULL) {
		return EXIT_FAILURE;
	}

	f = fopen(fname,"w");

	if(f==NULL) {
		return EXIT_FAILURE;
	}

	//Write first line
	fprintf(f,"digraph {\n");

	Markov_OutputBinaryTreeDot(f,m,m->root);

	//Write last line
	fprintf(f,"}\n");
	fclose(f);

	return EXIT_SUCCESS;
}
