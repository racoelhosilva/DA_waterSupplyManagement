//
// Created by rodrigo on 3/20/24.
//

#ifndef DA_WATERSUPPLYMANAGEMENT_ANSI_H
#define DA_WATERSUPPLYMANAGEMENT_ANSI_H

// Text Formatting
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define FAINT       "\033[2m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"
#define INVERT      "\033[7m"
#define HIDDEN      "\033[8m"

// Text Colors (Foreground)
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Background Colors
#define BGBLACK     "\033[40m"
#define BGRED       "\033[41m"
#define BGGREEN     "\033[42m"
#define BGYELLOW    "\033[43m"
#define BGBLUE      "\033[44m"
#define BGMAGENTA   "\033[45m"
#define BGCYAN      "\033[46m"
#define BGWHITE     "\033[47m"

// Cursor Control
#define HOME        "\033[H"
#define CLEAR       "\033[2J"
#define CLEARLINE   "\033[K"
#define MOVE_UP          "\033[A"
#define MOVE_DOWN        "\033[B"
#define MOVE_RIGHT       "\033[C"
#define MOVE_LEFT        "\033[D"



#endif //DA_WATERSUPPLYMANAGEMENT_ANSI_H