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

// Erase Characters
#define CLEAR_SCREEN        "\033[2J" // Clear entire screen
#define CLEAR_TO_START      "\033[1J" // Clear from cursor to beginning of screen
#define CLEAR_TO_END        "\033[0J" // Clear from cursor to end of screen
#define CLEAR_LINE          "\033[K"  // Clear from cursor to end of line
#define CLEAR_LINE_START    "\033[1K" // Clear from cursor to beginning of line
#define CLEAR_LINE_FULL     "\033[2K" // Clear entire line

// Move Cursor
#define MOVE_CURSOR(row,col) "\033[" #row ";" #col "H" // Move cursor to specific position (row, col)
#define MOVE_CURSOR_UP(n)    "\033[" #n "A"            // Move cursor up by n lines
#define MOVE_CURSOR_DOWN(n)  "\033[" #n "B"            // Move cursor down by n lines
#define MOVE_CURSOR_RIGHT(n) "\033[" #n "C"            // Move cursor right by n columns
#define MOVE_CURSOR_LEFT(n)  "\033[" #n "D"            // Move cursor left by n columns


#endif //DA_WATERSUPPLYMANAGEMENT_ANSI_H
