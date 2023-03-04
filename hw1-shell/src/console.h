#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "base.h"

void erase_console(const int count);
void print_line_format(const Env env, const Validity valid);
void empty_line(void);
void print_logo(void) ;
void print_welcome(void);

#endif //!__CONSOLE_H__