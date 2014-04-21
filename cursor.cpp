#include "brute.h"
int cursor::position() {
    //Gets position of the cursor
    int row, column;
    getyx(stdscr, row, column);
    column = column - PRELINE_SIZE - strlen(PRELINE_DELIMETER);

    //Sets position of the cursor to where it should be (if it was lagging behind)
    pos = column;
    return pos;
}

void cursor::right() {
    pos += 1;
}

void cursor::left() {
    if(pos > 0)
        pos -= 1;
}

void cursor::go_to(int p) {
    if(p > 0)
        pos = p;
}

cursor::cursor(int p):pos(p) {}
