/**
 * @file ansi.cpp
 * @brief Creates a macro-based library for ANSI escape sequences
 */

#ifndef DA_WATERSUPPLYMANAGEMENT_ANSI_H
#define DA_WATERSUPPLYMANAGEMENT_ANSI_H

// Text Formatting
#define RESET       "\033[0m" // Resets all text formatting
#define BOLD        "\033[1m" // Sets text to bold
#define FAINT       "\033[2m" // Sets text to faint intensity
#define ITALIC      "\033[3m" // Sets text to italic
#define UNDERLINE   "\033[4m" // Underlines text
#define BLINK       "\033[5m" // Makes text blink
#define INVERT      "\033[7m" // Inverts the foreground and background colors
#define HIDDEN      "\033[8m" // Hides text (not widely supported)

// Text Colors (Foreground)
#define DEFAULT     "\033[39m"    // Sets text color to default (usually terminal's default)
#define BLACK       "\033[30m"    // Sets text color to black
#define RED         "\033[31m"    // Sets text color to red
#define GREEN       "\033[32m"    // Sets text color to green
#define YELLOW      "\033[33m"    // Sets text color to yellow
#define BLUE        "\033[34m"    // Sets text color to blue
#define MAGENTA     "\033[35m"    // Sets text color to magenta
#define CYAN        "\033[36m"    // Sets text color to cyan
#define WHITE       "\033[37m"    // Sets text color to white
#define BRIGHT_BLACK   "\033[90m"   // Sets text color to bright black
#define BRIGHT_RED     "\033[91m"   // Sets text color to bright red
#define BRIGHT_GREEN   "\033[92m"   // Sets text color to bright green
#define BRIGHT_YELLOW  "\033[93m"   // Sets text color to bright yellow
#define BRIGHT_BLUE    "\033[94m"   // Sets text color to bright blue
#define BRIGHT_MAGENTA "\033[95m"   // Sets text color to bright magenta
#define BRIGHT_CYAN    "\033[96m"   // Sets text color to bright cyan
#define BRIGHT_WHITE   "\033[97m"   // Sets text color to bright white

// Background Colors
#define DEFAULT_BG  "\033[49m" // Sets background color to default (usually terminal's default)
#define BGBLACK     "\033[40m" // Sets background color to black
#define BGRED       "\033[41m" // Sets background color to red
#define BGGREEN     "\033[42m" // Sets background color to green
#define BGYELLOW    "\033[43m" // Sets background color to yellow
#define BGBLUE      "\033[44m" // Sets background color to blue
#define BGMAGENTA   "\033[45m" // Sets background color to magenta
#define BGCYAN      "\033[46m" // Sets background color to cyan
#define BGWHITE     "\033[47m" // Sets background color to white
#define BRIGHT_BGBLACK   "\033[100m" // Sets background color to bright black
#define BRIGHT_BGRED     "\033[101m" // Sets background color to bright red
#define BRIGHT_BGGREEN   "\033[102m" // Sets background color to bright green
#define BRIGHT_BGYELLOW  "\033[103m" // Sets background color to bright yellow
#define BRIGHT_BGBLUE    "\033[104m" // Sets background color to bright blue
#define BRIGHT_BGMAGENTA "\033[105m" // Sets background color to bright magenta
#define BRIGHT_BGCYAN    "\033[106m" // Sets background color to bright cyan
#define BRIGHT_BGWHITE   "\033[107m" // Sets background color to bright white


// Cursor Control
#define HOME        "\033[H"   // Moves cursor to the home position (1,1)
#define CLEAR       "\033[2J"  // Clears the entire screen
#define CLEARLINE   "\033[K"   // Clears the current line
#define MOVE_UP     "\033[A"   // Moves cursor up one line
#define MOVE_DOWN   "\033[B"   // Moves cursor down one line
#define MOVE_RIGHT  "\033[C"   // Moves cursor right one column
#define MOVE_LEFT   "\033[D"   // Moves cursor left one column


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
#define HIDE_CURSOR          "\033[?25l"               // Hides the cursor
#define SHOW_CURSOR          "\033[?25h"               // Shows the cursor
#define SAVE_CURSOR          "\033[s"                  // Saves current cursor position
#define RESTORE_CURSOR       "\033[u"                  // Restores cursor position previously saved by SAVE_CURSOR

#endif //DA_WATERSUPPLYMANAGEMENT_ANSI_H
