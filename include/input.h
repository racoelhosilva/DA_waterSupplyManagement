#ifndef DA_WATERSUPPLYMANAGEMENT_INPUT_H
#define DA_WATERSUPPLYMANAGEMENT_INPUT_H

#include <string>

enum Press { UP, DOWN, LEFT, RIGHT, RET, DELETE, CHAR };

void initCapture();
Press getNextPress();
void clearBuffer();
std::string getBuffer();
void endCapture();

#endif //DA_WATERSUPPLYMANAGEMENT_INPUT_H
