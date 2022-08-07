#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "program.h"
#include "vector.h"
#include "models.h"
#include "fileio.h"
#include "args.h"
#include "logger.h"

database_t *init_database(int argc, char *argv[]) {
    database_t *db = malloc(sizeof(database_t));
    if (!db) exit(1);

    collect_args(db, argc, argv);

    if (!db->name) {
        db->name = strdup("db");
        if (!db->name) exit(1);
    }

    printf("\n"
        "  Tsili Betshop, version 1.0.0\n"
        "  Copyright (C) 2022 Sakis, Inc.\n"
    );

    if (TEST_MODE) {
        printf("\n"
            "  You are currently using the application in test mode.\n"
            "  Thus you do not have any privilege to save new data.\n"
            "  Any new changes will be temporary and be lost after quiting the application.\n"
            "  To use the application in normal mode, please provide a username and a password.\n"
        );
        db->admin = strdup("test");
        if (!db->admin) exit(1);
    }

    db->players = new_vector(TYPE_PLAYER);
    db->matches = new_vector(TYPE_MATCH);
    db->bets = new_vector(TYPE_BET);

    if (!TEST_MODE) {
        load_database(db);
    }

    return db;
}

void main_loop(database_t *db) {
    char option;

    do {
        printf(ORANGE("%s@tsili") ":" YELLOW("%s") "$ ", db->admin, db->name);
        scanf("%c", &option);
        if (option == '\n') continue;

        switch (option) {
        case 'p':
            add_player(db);       
            break;
        case 'm':
            add_match(db);
            break;
        case 'b':
            add_bet(db);
            break;
        case 'r':
            add_result(db);
            break;
        case 't':
            print_tables(db);
            break;
        case 's':
            if (TEST_MODE) {
                error("You do not have the permision to save new data.\n"
                    "  Please provide a username and a password.\n"
                );
            } else {
                save_database(db);
                info("Data saved successfuly on " YELLOW("%s") ".\n", db->name);
            }
            break;
        case 'q':
            if (!TEST_MODE) {
                save_database(db);
            }
            break;
        case 'h':
            print_usage_msg();
            break;
        default:
            error("Invalid option.\n"
                "  Press " YELLOW("h") " to see the program usage.\n");
            break;
        }

        while (getchar() != '\n');
    } while (option != 'q');
}

void clean_up(database_t *db) {
    vector_clear(db->players);
    vector_clear(db->matches);
    vector_clear(db->bets);

    free(db->name);
    free(db->admin);

    free(db);
}

void add_player(database_t *db) {
    player_t *player;
    char *username;

    scanf("%ms", &username);

    player = new_player(username);
    vector_push(db->players, player);

    info("New player " YELLOW("%s") " added.\n", username);

    free(username); 
}

void add_match(database_t *db) {
    char *first_team, *second_team;
    match_t *match;
    
#if DEBUG
    static int id = 0;
#endif

    scanf("%ms %ms", &first_team, &second_team);

    match = new_match(first_team, second_team);
#if DEBUG
    match->id = id++;
#else
    match_set_id(match, db->matches);
#endif
    vector_push(db->matches, match);
    matches_sort_by_id(db->matches);

    info("New match " YELLOW("%s vs %s") " added [id=" YELLOW("%d") "].\n", first_team, second_team, match->id);

    free(first_team);
    free(second_team);
}

void add_bet(database_t *db) {
    char *username;
    int match_id, res;
    char choice;
    match_t *match;
    player_t *player;
    bet_t *bet;

    scanf("%ms %d %c", &username, &match_id, &choice);
    choice = toupper(choice);
    if (choice != ONE && choice != TWO && choice != TIE) {
        error("Invalid choice.\n");
        free(username);
        return;
    } 

    res = player_username_search_at(db->players, username);
    if (res == -1) {
        error("Player not found.\n");
        free(username);
        return;
    }

    player = vector_at(db->players, res);
    res = match_id_search_at(db->matches, match_id);
    if (res == -1) {
        error("Match not found.\n");
        free(username);
        return;
    }
    
    match = vector_at(db->matches, res);
    bet = new_bet(player, match, choice);
    vector_push(db->bets, bet);
    info("New bet successfuly added.\n"
        "  Player " YELLOW("%s") " played " YELLOW("%c") " on " YELLOW("%s vs %s") ".\n",
        bet->player->username, choice, bet->match->first_team, bet->match->second_team
    );

    free(username);
}

void add_result(database_t *db) {
    int match_id, res;
    char match_result;
    match_t *match;
    player_t *player;
    bet_t *bet;

    scanf("%d %c", &match_id, &match_result);
    match_result = toupper(match_result);
    if (match_result != ONE && match_result != TWO && match_result != TIE) {
        error("Invalid match result.\n");
        return;
    }

    res = match_id_search_at(db->matches, match_id);
    if (res == -1) {
        error("Match not found.\n");
        return;
    }

    match = vector_at(db->matches, res);
    match->done = 1;
    info("Match " YELLOW("%s vs %s") " resulted in " YELLOW("%c") ".\n",
        match->first_team, match->second_team, match_result);
    for (int i = 0; i < vector_count(db->bets); i++) {
        bet = vector_at(db->bets, i);
        if (bet->match == match) {
            bet->done = 1;
            player = bet->player;
            if (bet->choice == match_result) {
                if (match_result == TIE) {
                    player->points += 1;
                    printf("  Player " YELLOW("%10s") " played " YELLOW("%c") " and won  " 
                        YELLOW("1€") " [total=" YELLOW("%2d€") "].\n",
                        player->username, bet->choice, player->points);
                } else {
                    player->points += 3;
                    printf("  Player " YELLOW("%10s") " played " YELLOW("%c") " and won  " 
                        YELLOW("3€") " [total=" YELLOW("%2d€") "].\n",
                        player->username, bet->choice, player->points);
                }
            } else {
                player->points -= 1;
                printf("  Player " YELLOW("%10s") " played " YELLOW("%c") " and lost " 
                    YELLOW("1€") " [total=" YELLOW("%2d€") "].\n",
                    player->username, bet->choice, player->points);
            }
        }
    }
}

void print_tables(database_t *db) {
    players_sort_by_points(db->players);

    printf("\nPLAYERS:\n");
    for (int i = 0; i < vector_count(db->players); i++) {
        player_print(vector_at(db->players, i));
    }

    printf("##\n\nMATCHES:\n");
    for (int i = 0; i < vector_count(db->matches); i++) {
        match_print(vector_at(db->matches, i));
    }

    printf("##\n\nBETS:\n");
    for (int i = 0; i < vector_count(db->bets); i++) {
        bet_print(vector_at(db->bets, i));
    }

    printf("##\n");
}

void print_usage_msg() {
    printf(
        "\nUSAGE:\n"
        "  p <player username>                   add player\n"
        "  m <first team> <second team>          add match\n"
        "  b <player username> <match id> <bet>  add bet\n"
        "  r <match id> <match result>           add result\n"
        "  t                                     print tables\n"
        "  s                                     save data\n"
        "  q                                     quit\n"
        "##\n"
    );
}