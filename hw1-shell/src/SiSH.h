#ifndef __SISH_H__
#define __SISH_H__

#include <signal.h>
#include "base.h"
#include "execute.h"
#include "keyboard.h"
#include "console.h"

int sish_main(int argc, char* argv[]);

char* get_current_path(char* const current_path_abs, const char* const home_dir);
char* get_current_path_abs(const int path_max);

#endif //!__SISH_H__