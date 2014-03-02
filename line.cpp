#include "brute.h"
using namespace std;

line::line(int t = 0): tab(t), cur(0), start(0) {
    //Indents as much as needed
    for(int i=0; i<t; i++) {
        characters.push_back(' ');
    }
}

line::line(string c) :tab(0), characters(c), cur(0), start(0) {}

void line::insert(char c) {
    //Gets iterator using cursor position
    std::string::iterator iterator = getIterator();

    //Fills line with extra character
    characters.insert(iterator, c);
    cur.right();
}

void line::pop() {
    int row, column;
    //Gets iterator using cursor position
    std::string::iterator iterator = getIterator();
    getyx(stdscr, row, column);
    characters.erase(--iterator);
    move(row, column-1);
    addch(' ');
    move(row, column-1);
    cur.left();
}

char line::back(){
    //Returns last character
    int length = characters.size();
    return characters[length-1];
}

char line::front() {
    if(characters.empty())
        return '\0';

    //Returns first character
    return characters[0];
}

//Prints line without preline
void line::print() {
    int maxrow, maxcol;
    getmaxyx(stdscr, maxrow, maxcol);
    int length = maxcol - PRELINE_SIZE - strlen(PRELINE_DELIMETER);
    //Generates substring to print
    string sub = characters.substr(start, length);
    printw(sub.c_str());
}

bool line::eol() {
    //Gets iterator using cursor position
    std::string::iterator iterator = getIterator();
    if(iterator == characters.end())
        return true;
    else
        return false;
}

std::string::iterator line::getIterator() {
    //Gets position of the cursor
    /*int row, column;
    getyx(stdscr, row, column);
    column = column - PRELINE_SIZE - strlen(PRELINE_DELIMETER);*/
    int column = cur.position();
    
    //Finds position in string relative to cursor
    std::string::iterator iterator = characters.begin();
    for(int i=0; i<column+start; i++) {
        iterator++;
    }
    return iterator;
}

line line::split() {
    //Gets iterator using cursor position
    std::string::iterator iterator = getIterator();
    string newChars;

    while(iterator != characters.end()) {
        newChars.push_back(*iterator);
        characters.erase(iterator);
    }
    //Create new line and return it. 
    line newLine(newChars);
    return newLine;
}

bool line::can_go_left() {
    //If the line is empty, return false
    if(characters.empty())
        return false;

    //If the cursor is at the front, return false
    std::string::iterator iterator = getIterator();
    if(iterator == characters.begin())
        return false;
    
    return true;
} 

bool line::can_go_right() {
    //Get the iterator from the cursor position
    std::string::iterator iterator = getIterator();  

    if(iterator == characters.end())
        return false;
    if(*iterator == '\n')
        return false;

    return true;
}

string line::getLine() const{
    return characters;
}   

void line::scroll_right() {
    start++;
}

void line::scroll_left() {
    start--;
}

void line::combine(const line &newLine) {
    if(back() == '\n') {
        int last = characters.size()-1;
        characters[last] = ' ';
    }
    characters = characters + newLine.getLine();
}

inline bool operator==(const line& lhs, const line& rhs) {
    return lhs.getLine() == rhs.getLine();
}

inline bool operator!=(const line& lhs, const line& rhs) {
    return !(lhs == rhs);
}
    
    
    

