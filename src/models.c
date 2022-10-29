#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "models.h"
#include "vector.h"
#include "logger.h"

int choice_is_valid(char choice) {
    return choice == ONE || choice == TWO || choice == TIE;
}

match_t *new_match(char *first_team, char *second_team, double odds[3]) {
    match_t *match = malloc(sizeof(match_t));
    if (!match) exit(1);

    match->first_team = strdup(first_team);
    if (!match->first_team) exit(1);
    match->second_team = strdup(second_team);
    if (!match->second_team) exit(1);

    memcpy(match->odds, odds, 3 * sizeof(double));
    match->done = 0;

    return match;
}

player_t *new_player(char *username) {
    player_t *player = malloc(sizeof(player_t));
    if (!player) exit(1);

    player->username = strdup(username);
    if (!player->username) exit(1);
    player->points = 100;

    return player;
}

bet_t *new_bet(player_t *player, match_t *match, char choice, int amount) {
    bet_t *bet = malloc(sizeof(bet_t));
    if (!bet) exit(1);

    bet->player = player;
    bet->match = match;
    bet->choice = choice;
    bet->amount = amount;
    bet->done = 0;

    return bet;
}

void match_clear(match_t *match) {
    free(match->first_team);
    free(match->second_team);
    free(match);
}

void player_clear(player_t *player) {
    free(player->username);
    free(player);
}

void match_set_id(match_t *match, vector_t *vector) {
    srand(time(NULL));

    do {
        match->id = 100 + rand() % 300;
    } while (match_id_search_at(vector, match->id) != -1);
}

void matches_sort_by_id(vector_t *vector) {
    int i, j;

    for (i = 1; i < vector_count(vector); i++) {
		match_t *match_i = vector->table[i], *match_j;
		for (j = i - 1; j >= 0; j--) {
			match_j = vector->table[j];
			if (match_i->id > match_j->id) break;
			vector->table[j + 1] = match_j;
		}

		vector->table[j + 1] = match_i;
    }
}

void players_sort_by_points(vector_t *vector) {
    int i, j;

    for (i = 1; i < vector_count(vector); i++) {
		player_t *player_i = vector->table[i], *player_j;
		for (j = i - 1; j >= 0; j--) {
			player_j = vector->table[j];
			if (player_i->points < player_j->points) break;
			vector->table[j + 1] = player_j;
		}

		vector->table[j + 1] = player_i;
    }
}

int match_id_search_at(vector_t *vector, int id) {
    int from = 0, to = vector_count(vector) - 1, mid;

    while (from <= to) {
        mid = (from + to) / 2;

        match_t *match = vector_at(vector, mid);
        if (match->id == id) {
            return mid;
        } else if (match->id < id) {
            from = mid + 1;
        } else {
            to = mid - 1;
        }
    }

    return -1; 
}

int player_username_search_at(vector_t *vector, char *username) {
	for (int i = 0; i < vector_count(vector); i++) {
        player_t *player = vector_at(vector, i);
        if (strcmp(player->username, username) == 0) return i; 
	}

	return -1;
}

void match_print(match_t *match) {
    char format_str[80];

    if (match->done) {
        sprintf(format_str, BG_RED(BLACK("[%%d] %%%ds vs %%-%ds (%%.2lf, %%.2lf, %%.2lf)")) "\n", SPACING, SPACING);
    } else {
        sprintf(format_str, BG_GREEN(BLACK("[%%d] %%%ds vs %%-%ds (%%.2lf, %%.2lf, %%.2lf)")) "\n", SPACING, SPACING);
    }
    printf(format_str, match->id, match->first_team, match->second_team, match->odds[0], match->odds[1], match->odds[2]);
}

void player_print(player_t *player) {
    char format_str[40];

    sprintf(format_str, "%%%ds %%d€\n", SPACING);
    printf(format_str, player->username, player->points);
}

void bet_print(bet_t *bet) {
    char format_str[80];

    if (bet->done) {
        sprintf(format_str, BG_RED(BLACK("%%%ds: %%c (%%3d€) on %%%ds vs %%-%ds")) "\n", SPACING, SPACING, SPACING);
    } else {
        sprintf(format_str, BG_GREEN(BLACK("%%%ds: %%c (%%3d€) on %%%ds vs %%-%ds")) "\n", SPACING, SPACING, SPACING);
    }
    printf(format_str, bet->player->username, bet->choice, bet->amount, bet->match->first_team, bet->match->second_team);
}

void match_write_file(match_t *match, FILE *db_file) {
    fprintf(db_file, "m %d %s %s %.2lf %.2lf %.2lf\n",
        match->id, match->first_team, match->second_team, match->odds[0], match->odds[1], match->odds[2]);
}

void player_write_file(player_t *player, FILE *db_file) {
    fprintf(db_file, "p %s %d\n", player->username, player->points);
}

void bet_write_file(bet_t *bet, FILE *db_file) {
    fprintf(db_file, "b %s %d %c %d\n", bet->player->username, bet->match->id, bet->choice, bet->amount);
}