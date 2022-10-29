# Tsili Betshop

A simple betting CLI application written in C.

## Build:

Open your terminal in the directory where you cloned the repository and write:

```bash
mkdir objs backup
make
```

## Running:

The program runs as follows:

```bash
./tsili [ -d <dbname> ] [ -u <admin> ]
```

where \<dbname> is the relative path to the database and \<admin> is the username of the database administrator. 
Then a password is requested (you can look up for the admins and their password inside the src/args.c file).

If the -d flag wont be used, the default "db" database will bey created or accessed.

If the -u flag wont be used, the app will run in <b>test mode</b> and you wont be able to save data in any database.

Inside the app you can type h to see its usage.