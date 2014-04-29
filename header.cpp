#include "brute.h"
using namespace std;

//Default constructor
header::header(string &fname):filename(fname), saved(true), delimeter('-'), headerType(DEFAULT) {}

void header::unsave() {
    saved = false;
}

void header::save() {
    saved = true;
}

bool header::is_saved(){
    return saved;
}

void header::print() {        
    //Print the name of the file we are working on
    printw(filename.c_str());

    //Get maximum limits of the screen
    int maxr;
    int maxc;
    getmaxyx(stdscr, maxr, maxc);

    int row;
    int col;
    getyx(stdscr, row, col);

    //Determine the type of header that needs to be printed
    if(headerType == SAVECHECK) {
        string question = "Do you want to quit without saving?\n";
        string answer = "y: quit n: cancel\n";

        while(col < maxc - 1 - question.size()) {
            addch(' ');
            col++;
        }
        printw(question.c_str());
        col = 0;

        while(col < maxc - 1 - answer.size()) {
            addch(' ');
            col++;
        }
        printw(answer.c_str());
    }
    else {    
        //Print whether the file is saved
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
    }

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

void header::changeHeaderType(int type) {
    headerType = type;
}
