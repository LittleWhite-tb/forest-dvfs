#ifndef H_MPI_REPLACE
#define H_MPI_REPLACE


typedef double (*evalGet)(void *data);
typedef  void* (*evalInit)(void);
typedef int (*evalClose)(void *data);

void *dlPower;
void *dlTime;


/*tempral function pointers*/
evalGet tmpEvaluationStart;
evalGet tmpEvaluationStop;
evalInit tmpEvaluationInit;
evalClose tmpEvaluationClose;


/**
 * @enum Time
 * @brief contain all the funcion pointers needed to init/close and start/stop the time counter*/
typedef struct Time
{
	evalGet evaluationStart;   /**< @brief counter start function*/
	evalGet evaluationStop;    /**< @brief counter stop function*/
	evalInit evaluationInit;   /**< @brief counter Initialisation function*/
	evalClose evaluationClose; /**< @brief counter Close function*/
        double start;		   /**< @brief store the current number of the CPU ticks when the counter start function was called*/
        double stop;		   /**< @brief store the current number of the CPU ticks when the counter stop function was called*/
        double elapsed;		   /**< @brief store the number of elapsed CPU ticks between the start and the stop */
}Time;

/**
 * @enum Energy
 * @brief cointain all the function pointers needed to init/close the Enery server, and get values from it*/
typedef struct Energy
{
	evalGet evaluationStart;   /**< @brief counter start function*/
	evalGet evaluationStop;    /**< @brief counter stop function*/
	evalInit evaluationInit;   /**< @brief counter Initialisation function*/
	evalClose evaluationClose; /**< @brief counter Close function*/
        double start;              /**< @brief store the current number of energy consumed when the counter start function was called*/
        double stop;               /**< @brief store the current number of energy consumed when the counter stop function was called*/
        double elapsed;            /**< @brief store the number of elapsed energy consumtion between the start and the stop */

}Energy;

Energy E;
Time T;



#endif
