 #ifndef REST_DECISION
#define REST_DECISION

#include "camus_definitions.h"
#include "camus_decision_support.h"
#include "camus_decision_transaction.h"

void camus_bind (camus_module_t module);


void restDecisionInit(camus_module_t module);
void restDecisionQuit(camus_module_t module);
void restDecisionOn(camus_module_t module);
void restDecisionOff(camus_module_t module);
void restDecisionReset(camus_module_t module);

int camus_decision_chunk(void* self, int* lower_bound, int* upper_bound);

#endif
