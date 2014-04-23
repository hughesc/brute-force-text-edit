#include "brute.h"
using namespace std;

//Default constructor
header::header() {
    contents = "Text Editor\n^Q: quit ^S: save\n";
    delimeter = '-';
}

void header::print() {
    //Print contents to the screen
    printw(contents.c_str());

    //Get maximum limits of the screen
    int maxr;
    int maxc;
    getmaxyx(stdscr, maxr, maxc);

    //Repeat the delimeter until the end of the screen
    int col = 0;
    while(col < maxc-1) {
        addch(delimeter);
        col ++;
    }
    addch('\n');
}

int header::size() {
    int count = 0;
    int index = 0;

    while(true) {
        index = contents.find('\n', index+1);
        if(index != string::npos)
            count++;
        else
            break;
    }
    
    //Make sure to account for the delimeter as well
    return count+1;
}
