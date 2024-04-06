#ifndef DA_WATERSUPPLYMANAGEMENT_INPUT_H
#define DA_WATERSUPPLYMANAGEMENT_INPUT_H

#include <string>

enum Press { UP, DOWN, LEFT, RIGHT, RET, DELETE, CHAR };

/**
 * @brief Starts capturing the keys
 */
void initCapture();

/**
 * @brief Reads one key from the input
 * @return The enum value corresponding to the key pressed
 */
Press getNextPress();

/**
 * @brief Clears the buffer of input text
 */
void clearBuffer();

/**
 * @brief Returns the buffer of input text
 * @return String with the input text
 */
std::string getBuffer();

/**
 * @brief Terminates the key capture from the program
 */
void endCapture();

#endif //DA_WATERSUPPLYMANAGEMENT_INPUT_H
