#ifndef H_LOG
#define H_LOG

#include <stdio.h>

/*
 * @brief Set the log value which determines verbosity
 * @param value the value of the verbosity that we accept
 */
void Log_setVerbosity (int value);

/*
 * @brief Set the output of the log
 * @param f the output we wish to use, by default this is stderr
 */
void Log_setOutput (FILE *f);

/*
 * @brief Log something
 * @param verbosity the value of the verbosity that we are using (0< will always log, otherwise it depends of what is set by Log_setVerbosity)
 * @param fmt format of the text we wish to print out
 */
void Log_output (int verbosity, char *fmt, ...);

#endif
