#include "input.h"

#ifdef __unix__
#include <termio.h>
#include <unistd.h>
#define getchr() getchar()
#define ENTER 10
#define ISARROW(c) (c == 27 && getchr() == 91)
#define UP_ARROW 65
#define DOWN_ARROW 66
#define RIGHT_ARROW 67
#define LEFT_ARROW 68
#define DEL 127
struct termios old_tio;
#else  // windows
#include <conio.h>
#define getchr() _getch()
#define ENTER 13
#define ISARROW(c) (c == -32 || c == 0)
#define UP_ARROW 72
#define DOWN_ARROW 80
#define DEL 83
#endif

#define BUFFER_SIZE 64
char buffer[BUFFER_SIZE + 1] = { '\0' };
int index = 0;

using namespace std;

void initCapture() {
    #ifdef __unix__
    struct termios new_tio;
    tcgetattr(STDIN_FILENO,&old_tio);
    new_tio=old_tio;
    new_tio.c_lflag &=(~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
    #else
    system("chcp 65001 > nul");
    #endif
}

Press getNextPress() {
    char c;
    while (true) {
        c = getchr();
        if (c == ENTER)
            return RET;
        if (ISARROW(c)) {
            switch (getchr()) {
                case UP_ARROW:
                    return UP;
                case DOWN_ARROW:
                    return DOWN;
                case RIGHT_ARROW:
                    return RIGHT;
                case LEFT_ARROW:
                    return LEFT;
            }
        }

        if (c == DEL) {
            if (index > 0)
                buffer[--index] = '\0';
            return DELETE;
        }
        if (index < BUFFER_SIZE)
            buffer[index++] = c;
        return CHAR;
    }
}

void clearBuffer() {
    for (int i = 0; i < BUFFER_SIZE + 1; i++)
        buffer[i] = '\0';
    index = 0;
}

string getBuffer() {
    return { buffer };
}

void endCapture() {
    #ifdef __unix__
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
    #endif
}
