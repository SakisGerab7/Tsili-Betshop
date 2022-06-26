#ifndef _ARGS_H_
#define _ARGS_H_

#include "program.h"

void collect_args(database_t *db, int argc, char *argv[]);
int admin_is_valid(char *admin);

#endif