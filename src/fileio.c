#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"
#include "program.h"
#include "vector.h"
#include "models.h"
#include "logger.h"

void save_database(database_t *db) {
    match_t *match;
    bet_t *bet;
    FILE *db_file = fopen(db->name, "w");
    if (!db_file) exit(1);

    fprintf(db_file, "DB:TSILI\n");

    for (int i = 0; i < vector_count(db->players); i++) {
        player_write_file(vector_at(db->players, i), db_file);
    }

    for (int i = 0; i < vector_count(db->matches); i++) {
        match = vector_at(db->matches, i);
        if (!match->done) match_write_file(match, db_file);
    }

    for (int i = 0; i < vector_count(db->bets); i++) {
        bet = vector_at(db->bets, i);
        if (!bet->done) bet_write_file(bet, db_file);
    }

    fclose(db_file);
}

void load_database(database_t *db) {
    char option, choice;
    char *username, *first_team, *second_team, *magic_key;
    int points, match_id;
    player_t *player;
    match_t *match;
    bet_t *bet;
    FILE *db_file = fopen(db->name, "r");
    if (!db_file) {
        db_file = fopen(db->name, "w");
        if (!db_file) exit(1);
        fprintf(db_file, "DB:TSILI\n");
        fclose(db_file);
        return;
    }

    fscanf(db_file, "%ms", &magic_key);
    if (strcmp(magic_key, "DB:TSILI") != 0) {
        error("Invalid database file.\n");
        free(magic_key);
        fclose(db_file);
        clean_up(db);
        exit(1);
    }

    free(magic_key);

    while (fscanf(db_file, " %c", &option) != EOF) {
        switch (option) {
        case 'p':
            fscanf(db_file, "%ms %d", &username, &points);
            player = new_player(username);
            player->points = points;
            vector_push(db->players, player);
            free(username);
            break;        
        case 'm':
            fscanf(db_file, "%d %ms %ms", &match_id, &first_team, &second_team);
            match = new_match(first_team, second_team);
            match->id = match_id;
            vector_push(db->matches, match);
            free(first_team);
            free(second_team);
            break;
        default:
            fscanf(db_file, "%ms %d %c", &username, &match_id, &choice);
            player = vector_at(db->players, player_username_search_at(db->players, username));
            match = vector_at(db->matches, match_id_search_at(db->matches, match_id));
            bet = new_bet(player, match, choice);
            vector_push(db->bets, bet);
            free(username);
            break;
        }
    }

    fclose(db_file);
}