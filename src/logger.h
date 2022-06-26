#ifndef LOGGER_H
#define LOGGER_H


#define BLACK   "\e[38;5;8m"
#define RED     "\e[38;5;9m"
#define GREEN   "\e[38;5;10m"
#define YELLOW  "\e[38;5;11m"
#define BLUE    "\e[38;5;12m"
#define MAGENTA "\e[38;5;13m"
#define CYAN    "\e[38;5;14m"
#define ORANGE  "\e[38;5;202m"
#define RESET   "\e[38;5;15m"

void error(const char *fstr, ...);
void info(const char *fstr, ...);

#endif