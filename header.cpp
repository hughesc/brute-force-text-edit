#include "brute.h"
using namespace std;

//Default constructor
header::header(string &fname):filename(fname), saved(true), delimeter('-') {}

void header::unsave() {
    saved = false;
}

void header::save() {
    saved = true;
}

void header::print() {
    printw(filename.c_str());

    //Get maximum limits of the screen
    int maxr;
    int maxc;
    getmaxyx(stdscr, maxr, maxc);

    //Print whether the file is saved
    int row;
    int col;
    getyx(stdscr, row, col);
    string indicator;

    if(saved)
        indicator = "Saved\n";
    else
        indicator = "Unsaved\n";

    while(col < maxc - 1 - indicator.size()) {
        addch(' ');
        col++;
    }
    printw(indicator.c_str());

    //Print controls
    printw("^Q: quit ^S: save\n");

    //Repeat the delimeter until the end of the screen
    col = 0;
    while(col < maxc-1) {
        addch(delimeter);
        col ++;
    }
    addch('\n');
}

int header::size() {
    /*int count = 0;
    int index = 0;

    while(true) {
        index = contents.find('\n', index+1);
        if(index != string::npos)
            count++;
        else
            break;
    }
    
    //Make sure to account for the delimeter as well
    return count+1;*/
    //Hard code the number because it won't change by itself
    return 3; 
}
