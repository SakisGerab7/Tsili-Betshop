#include "program.h"

int main(int argc, char *argv[]) {
    database_t db;
    init_database(&db, argc, argv);
    main_loop(&db);
    clean_up(&db);

    return 0;
}
