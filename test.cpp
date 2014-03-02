#include <ncurses.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main() {
    initscr();
    keypad(stdscr, true);
    noecho();
    raw();

    int c;
    stringstream ss;
    string str;
    while(true) {
        int c = getch();
        if(c == 17)
            break;
        ss << c;
        str = ss.str();
        clear();
        printw(str.c_str());
        refresh();
    }
}
