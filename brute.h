#ifndef BRUTE_H
#define BRUTE_H

#include <ncurses.h>
#include <string>
#include <cstring>
#include <sstream>
#include <list>
#include <fstream>
#include <stdlib.h>
using namespace std;

//Preprocesser Definitions
//Define preline stuff so that it can be easily changed
#define PRELINE_SIZE 4
#define PRELINE_DELIMETER " |"

//Define extra keys that are not defined by ncurses.h
#define KEY_CNTL_Q 17   //The control-Q character
#define KEY_CNTL_S 19   //The control-S character
#define KEY_CNTL_C 3    //The control-c character
#define KEY_CNTL_V 22   //The control-v character

class preline {
    public:
        //prints the preline
        void print(int);
        
        //Constructor. Does not take a parameter. Empty until print is called
        preline();
    private:
        string characters;
};

class cursor {
    public:
        int position();
        void right();
        void left();
        void go_to(int);
        cursor(int);
    private:
        int pos;
};

//Holds one line of text along with the tab and line number
//push_back(), pop(), and constructor prints to screen
class line {
    public:
        //Adds one more character to the line
        void insert(char c);
        //Deletes character from line
        void pop();
        //Returns first character
        char front();
        //Returns last character
        char back();
        //Prints each line along with preline
        void print();
        //Boolean function that returns true if cursor is at the end
        bool eol();
        //Returns iterator based on cursor position
        std::string::iterator getIterator();
        //Cuts a line. First stays as original object, second is returned as second object
        line split();
        //Returns boolean as to whether the cursor can travel left
        bool can_go_left();
        //Returns boolean as to whether the cursor can travel right
        bool can_go_right();
        //Scrolls the line right if the line is too long to display on screen
        void scroll_right();
        //Scrolls the line left if the line is too long to display on screen
        void scroll_left();
        //Returns string held by line
        string getLine() const;
        //Returns cursor position
        int cursorPos();
        //Adds a second line's contents to the line
        void combine(const line &newLine);
        //Returns length of line w/ preline
        int size() const;
        //Returns tab level of line
        int tabLevel();
        //Creates empty line with tab length. Tab length defaults to zero
        line(int);
        //Creates line with contents of string passed.
        line(string);

    private:
        int tab;
        string characters;
        int start;
        cursor cur;
};

//Window pane class. Controls lines through linked list
class pane {
    public:
        //Creates new pane with title string
        pane(string);
        //Lets the window control what happens
        void take_control();
        //Refills window with relevant information 
        void refill_from(int);
        //Initializes ncurses and the window we will be using
        void initialize();
        //Saves file line by line
        void save(const string &filename);

    private:
        list<line> doc;
        string fileName;
};

class err {
    public:
        string what() {return descriptor;}
        err(string s): descriptor(s) {};
    private:
        string descriptor;
};

#endif
