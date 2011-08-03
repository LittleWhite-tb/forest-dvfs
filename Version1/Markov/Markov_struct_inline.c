#include "Markov_struct.c"

void Markov_Wait(SMarkov *m, SGlobalAdd *info)
{
		//search for a node labeled info in the binary tree (Markov_SearchNode returns the associated prediction node)
		m->cur[0] = Markov_SearchNode(info,m->root);

		if(m->cur[0])
			m->fct = &Markov_Predict;
		
		//increment the number of consecutive mistakes
  		m->nbr_err++;
		//if this number exceeds a threshold, we flush the graph and restart
		if(m->nbr_err>m->nbr_err_max) {
			Markov_Reset(m);
			return;
			}
}

//predict is the function for the prediction phase
inline static void Markov_Predict_Inline(SMarkov* m,SGlobalAdd* info)
{
	SNode* tmp ,*tmp2;
	SNodeList* ltmp;

	int max_vis;
	int itmp;

	//Solution to call the fct pointer if we're not in the prediction mode
        if(m->fct != &Markov_Predict)
                {
                m->fct(m,info);
                return;
                }

	//Our position in the graph of prediction is always stored in m->cur[0]
	tmp = m->cur[0];

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
			return;
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
			} 

			//we increment the number of mistakes
			m->nbr_err++;
			//if nbr_err is over the threshold
			if(m->nbr_err>m->nbr_err_max) {
				Markov_Reset(m);
				return;
			}
			return;
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
			}

			//increment the number of consecutive mistakes
			m->nbr_err++;
			//if nbr_err is over the threshold
			if(m->nbr_err>m->nbr_err_max) {
				Markov_Reset(m);
				return;
			}

			//we predict
			return;
		}

		//We check the other sons
		ltmp = tmp->next;

		while(ltmp != NULL) {
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
				Markov_Reset(m);
				return;
			}

		//we try to find a root node with the same label
   		m->cur[0] = Markov_SearchNode(info,m->root);
		
		if(m->cur[0]==NULL) {
			m->fct = &Markov_Wait;
			}
		else {
			//if we do find one, we will use tmp->last to link us to it
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
		}
}

