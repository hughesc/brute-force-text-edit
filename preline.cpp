#include "brute.h"
using namespace std;

preline::preline() {  
}

//Prints the preline to screen
void preline::print(int n) {
    ostringstream conversion;
    conversion << n;
    characters = conversion.str();

    //Makes the preline a standard size. Default is 4.
    while(characters.size() < PRELINE_SIZE) {
        characters.push_back(' ');
    }
    characters += PRELINE_DELIMETER;
    printw(characters.c_str());
}

