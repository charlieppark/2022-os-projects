#ifndef __ERROR_H__
#define __ERROR_H__

#include "base.h"
#include <errno.h>
#include <string.h>

void print_error(const char* const func_name, const char* const message);

#endif //!__ERROR_H__