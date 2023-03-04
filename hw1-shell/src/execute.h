#ifndef __EXECUTE_H__
#define __EXECUTE_H__

#include <sys/wait.h>
#include <sys/mman.h>
#include "base.h"
#include "parser.h"
#include "SiSH.h"

int execute(char* command, char** arr, const int pipe_index);
Validity cd(const char* const target_dir, Env* const env);
void history_save(const Env env, const char* const command);
char* dollar(const char* const str);

#endif //!__EXECUTE_H__