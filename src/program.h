#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "vector.h"

#define TEST_MODE (db->test_mode)

typedef struct {
    char *name;
    char *admin;
    int test_mode;
    vector_t *players;
    vector_t *matches;
    vector_t *bets;
} database_t;

database_t *init_database(int argc, char *argv[]);
void main_loop(database_t *db);
void clean_up(database_t *db);

void add_player(database_t *db);
void add_match(database_t *db);
void add_bet(database_t *db);
void add_result(database_t *db);
void print_tables(database_t *db);
void print_usage_msg();

#endif