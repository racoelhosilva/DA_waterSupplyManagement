//
// Created by bruno on 3/8/24.
//

#ifndef DA_WATERSUPPLYMANAGEMENT_INPUT_H
#define DA_WATERSUPPLYMANAGEMENT_INPUT_H

enum Press { NONE, UP, DOWN, RET };

void initCapture();
Press getNextPress();
void endCapture();

#endif //DA_WATERSUPPLYMANAGEMENT_INPUT_H
