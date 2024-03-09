#include "input.h"

#include <iostream>

#ifdef __unix__
#include <termio.h>
#include <unistd.h>
#define getchr() getchar()
#define ENTER 10
#define ISARROW(c) (c == 27 && getchr() == 91)
#define UP_ARROW 65
#define DOWN_ARROW 66
#define DEL 127
struct termios old_tio;
#else  // windows
#include <conio.h>
#define getchr() _getch()
#define ENTER 13
#define ISARROW(c) (c == -32)
#define UP_ARROW 65
#define DOWN_ARROW 66
#define DEL 8
#endif



void initCapture() {
    #ifdef __unix__
    struct termios new_tio;
    tcgetattr(STDIN_FILENO,&old_tio);
    new_tio=old_tio;
    new_tio.c_lflag &=(~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
    #endif
}

Press getNextPress() {
    char c;
    while (true) {
        c = getchr();
        if (c == ENTER)
            return RET;
        else if (ISARROW(c)) {
            switch (getchr()) {
                case UP_ARROW:
                    return UP;
                case DOWN_ARROW:
                    return DOWN;
            }
        }
    }
}

void endCapture() {
    #ifdef __unix__
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
    #endif
}
