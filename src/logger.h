#ifndef LOGGER_H
#define LOGGER_H

#define BLACK(x)   "\e[38;5;0m"   x "\e[39m"
#define RED(x)     "\e[38;5;9m"   x "\e[39m"
#define GREEN(x)   "\e[38;5;10m"  x "\e[39m"
#define YELLOW(x)  "\e[38;5;11m"  x "\e[39m"
#define BLUE(x)    "\e[38;5;12m"  x "\e[39m"
#define MAGENTA(x) "\e[38;5;13m"  x "\e[39m"
#define CYAN(x)    "\e[38;5;14m"  x "\e[39m"
#define ORANGE(x)  "\e[38;5;202m" x "\e[39m"

#define BG_BLACK(x)   "\e[48;5;8m"   x "\e[49m"
#define BG_RED(x)     "\e[48;5;9m"   x "\e[49m"
#define BG_GREEN(x)   "\e[48;5;10m"  x "\e[49m"
#define BG_YELLOW(x)  "\e[48;5;11m"  x "\e[49m"
#define BG_BLUE(x)    "\e[48;5;12m"  x "\e[49m"
#define BG_MAGENTA(x) "\e[48;5;13m"  x "\e[49m"
#define BG_CYAN(x)    "\e[48;5;14m"  x "\e[49m"
#define BG_ORANGE(x)  "\e[48;5;202m" x "\e[49m"

void error(const char *fstr, ...);
void info(const char *fstr, ...);

#endif