#include "Terminal.h"
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

static struct termios oldt, newt;

void initTermios() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void resetTermios() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

bool kbhit() {
    timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}
