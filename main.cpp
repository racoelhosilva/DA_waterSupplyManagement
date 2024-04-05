#include "include/Interface.h"

int main() {
    Interface interface;
    if (!interface.init()){
        return 1;
    }
    while(true){
        interface.mainMenu();
    }
}
