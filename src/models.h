#ifndef _MODELS_H_
#define _MODELS_H_

#include "vector.h"

#define SPACING 10

typedef struct {
    int id, done; 
    char *first_team, *second_team;
} match_t;

typedef struct {
    int points;
    char *username;
} player_t;

typedef struct {
    int done;
    player_t *player;
    match_t *match;
    enum {
        ONE = '1', TWO = '2', TIE = 'X'
    } choice;
} bet_t;

match_t *new_match(char *first_team, char *second_team);
player_t *new_player(char *username);
bet_t *new_bet(player_t *player, match_t *match, char choice);

void match_clear(match_t *match);
void player_clear(player_t *player);

void match_set_id(match_t *match, vector_t *vector);

void matches_sort_by_id(vector_t *vector);
void players_sort_by_points(vector_t *vector);

int match_id_search_at(vector_t *vector, int id);
int player_username_search_at(vector_t *vector, char *username);

void match_print(match_t *match);
void player_print(player_t *player);
void bet_print(bet_t *bet);

void player_write_file(player_t *player, FILE *db_file);
void match_write_file(match_t *match, FILE *db_file);
void bet_write_file(bet_t *bet, FILE *db_file);

#endif