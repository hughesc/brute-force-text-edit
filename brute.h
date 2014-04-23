#ifndef BRUTE_H
#define BRUTE_H

#include <ncurses.h>
#include <string>
#include <cstring>
#include <sstream>
#include <list>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <cctype>
#include "fileType.h"
#include <iostream>
using namespace std;

//Preprocesser Definitions
//Define preline stuff so that it can be easily changed
#define PRELINE_SIZE 4
#define PRELINE_DELIMETER " |"

//Define color macros
#define BLACK 1
#define BLUE 2
#define RED 3
#define GREEN 4

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
        syntax_tuple print(fileType, syntax_tuple);
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
        //Returns type of line
        int checkType() const;
        //Creates empty line with tab length. Tab length defaults to zero
        line(int);
        //Creates line with contents of string passed.
        line(string);
        //Creates line with contents of string and tabLvl
        line(string, int);

    private:
        int tab;
        string characters;
        int start;
        cursor cur;
        int type;
};

class header {
    public:
        //Constructer
        header();
	//Prints the header
        void print();
	//Returns the number of rows 
	int size();
    private:
        string contents;
	char delimeter;
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
        fileType ft;
        header textHeader;
};

class err {
    public:
        string what() {return descriptor;}
        err(string s): descriptor(s) {};
    private:
        string descriptor;
};       

#endif
