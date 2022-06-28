#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "program.h"
#include "logger.h"

void collect_args(database_t *db, int argc, char *argv[]) {
    int res;

    if (argc != 1 && argc != 3 && argc != 5) {
        error("Usage: %s [ -d <dbname> ] [ -u <admin> ]\n", argv[0]);
        exit(1);
    }

    db->name = NULL;
    db->admin = NULL;
    db->test_mode = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            i++;
            db->name = strdup(argv[i]);
            if (!db->name) exit(1);
        }

        if (strcmp(argv[i], "-u") == 0) {
            i++;
            db->admin = strdup(argv[i]);
            if (!db->admin) exit(1);

            res = admin_is_valid(db->admin);
            if (res == -1) {
                error("No such admin registered\n");
                exit(1);
            }

            if (res == -2) {
                error("Invalid password\n");
                exit(1);
            }

            db->test_mode = 0;
        }
    }
}

int admin_is_valid(char *admin) {
    const int num_admins = 2;
    int res;
    char *password;
    char *admins[][2] = {
        // Admins  // Passwords
        { "trompas", "obeoseinaigay"    },
        { "xaxas",   "reseisaitrelosre" }
    };

    for (int i = 0; i < num_admins; i++) {
        if (strcmp(admin, admins[i][0]) == 0) {
            printf("[password for %s]: ", admin);
            scanf("%ms", &password);
            while (getchar() != '\n');

            res = strcmp(password, admins[i][1]);
            free(password);
            return res == 0 ? 0 : -2;
        }
    }

    return -1;
}