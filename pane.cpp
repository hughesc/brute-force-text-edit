#include "brute.h"
using namespace std;

pane::pane(string title):fileName(title), ft(title.substr(title.find('.'))) {
    string loadedLine;

    fstream openedFile;
    openedFile.open(title.c_str(), ios::in | ios::app);
    if(openedFile.is_open()) {
        while(!openedFile.eof()) {
            getline(openedFile, loadedLine);
            doc.push_back(line(loadedLine+'\n'));
        }
        openedFile.close();
    }
    else {
        throw err("Could not open " + title);
    }
}

void pane::initialize() {
    //Initialize ncurses window to have keyboard inputs,
    //a getch function that doesnt echo, and raw input
    initscr();
    keypad(stdscr, true);
    noecho();
    raw();

    //Initialize color stuff
    if(has_colors()) {
        start_color();
        init_pair(BLACK, COLOR_WHITE, COLOR_BLACK);
        init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(RED, COLOR_RED, COLOR_BLACK);
        wbkgd(stdscr, COLOR_PAIR(BLACK));
    }

    refill_from(1);
    move(textHeader.size(), PRELINE_SIZE + strlen(PRELINE_DELIMETER));
}

void pane::take_control() {
    int c, row, column, maxrow, maxcol, prevmaxr, prevmaxc;
    int lineNum = 1, 
        tabLvl = 0;

    //Iterator to hold the working line starts at beginning of document
    std::list<line>::iterator workingLine = doc.begin();
    std::list<line>::iterator previous;
    //Int that holds number of top line
    int starter = 1;

    while(true){
        //Useful to get y and x positions each time
        getmaxyx(stdscr, maxrow, maxcol);
        getyx(stdscr, row, column);
        //c holds the input from the keyboard
        c = getch();

        //Quit if key is control-Q
        if(c == KEY_CNTL_Q)
            break;

        switch(c) {
            //Need to check if it can delete anything
            case KEY_BACKSPACE:
                if(workingLine->can_go_left()) {
                    workingLine->pop();
                    column--;
                    if(!(column >= PRELINE_SIZE + strlen(PRELINE_DELIMETER))) {
                        column++;
                        workingLine->scroll_left();
                    }
                }
                //If we are at beginning of line, delete the line
                else if(workingLine != doc.begin()) {
                    //First copy it into previous line
                    previous = workingLine;
                    previous--;
                    previous->combine(*workingLine);
                    //Now, erase the line
                    previous = workingLine;
                    workingLine++;
                    doc.erase(previous);
                    row--;
                    workingLine--;
                    column = workingLine->size()-1;
                }
                //Check if we need to scroll screen
                if(row < textHeader.size()) {
                    starter -= 1;
                    row = textHeader.size();
                }
                break;
            case KEY_DOWN:
                previous = workingLine;
                workingLine++;
                //Checks if we can go down
                if(workingLine != doc.end()) {
                    //Moves cursor down, changes working line
                    row += 1;
                    if(column > workingLine->size()-1)
                        column = workingLine->size() - 1;
                    else 
                        column = workingLine->cursorPos() + PRELINE_SIZE + strlen(PRELINE_DELIMETER);
                    //Check if we need to scroll screen
                    if(row >= maxrow)
                        starter += 1;
                }
                else {
                    workingLine = previous;
                }
                break;
            case KEY_UP:
                //Checks if we can move
                if(workingLine != doc.begin()) {
                    //Moves cursor and working line
                    row -= 1;
                    workingLine--;
                    if(column > workingLine->size()-1)
                        column = workingLine->size() - 1;
                    else 
                        column = workingLine->cursorPos() + PRELINE_SIZE + strlen(PRELINE_DELIMETER);
                    
                    //Check if we need to scroll screen
                    if(row < textHeader.size()) {
                        starter -= 1;
                        row = textHeader.size();
                    }
                }
                break;
            case KEY_LEFT: 
                //Check if we can scroll
                if(column > PRELINE_SIZE + strlen(PRELINE_DELIMETER)) {
                    column -= 1;
                }
                //Check if we need to scroll line
                else if(workingLine->can_go_left())
                    workingLine->scroll_left();
                //Check if we can travel up a line
                else if(workingLine != doc.begin()) {
                    row--;
                    workingLine--;
                    column = workingLine->size()-1;
                }
		break;
	    case KEY_RIGHT:
                //Check if we are at end of line. Do not scroll if at end of line
                if(workingLine->can_go_right()) {
                    column += 1;
                    //If we are all the way to right, scroll right, but don't add the extra column
                    if(column > maxcol-1) {
                        workingLine->scroll_right();
                        column -= 1;
                    }
                }
                else if(workingLine != --doc.end()) {
                    workingLine++;
                    row++;
                    column = PRELINE_SIZE + strlen(PRELINE_DELIMETER);
                }
                break;
            case KEY_RESIZE:
                //We don't need to do anything because the window will refresh itself after the break
                break;
            //When we press enter, we first create newline on old line
            case '\n':
                //Must check to make sure we are not in middle of a line. If we are, grab remainder of the workingLine
		//And bring it to the new line
                if(!workingLine->eol()) {
                    //Create a placeholder "previous"
                    previous = workingLine;
                    //Get tab level of workingLine
                    tabLvl = workingLine->tabLevel();
                    //Increment workingLine for insert function
                    workingLine++;
                    //Insert a new line with the second half of the old workingLine
                    doc.insert(workingLine, line(previous->split().getLine(), tabLvl));
                    previous->insert(char(c));
                    workingLine--;
                    //Set column so that we don't have to move cursor
                    column = tabLvl + PRELINE_SIZE + strlen(PRELINE_DELIMETER);
                }
                else {
                    workingLine->insert(char(c));
                    //Get tab level of workingLine
                    tabLvl = workingLine->tabLevel();
                    //Must increment workingLine to insert before it
                    workingLine++;
                    doc.insert(workingLine, line(tabLvl));
                    //Decrement workingLine to avoid dereferencing end of list
                    workingLine--;
                    column = PRELINE_SIZE + strlen(PRELINE_DELIMETER);
                }
                row++;
                //If we need to scroll screen, scroll it
                if(row >= maxrow)
                    starter += 1;
                break;
            case KEY_CNTL_S:
                save(fileName);
                break;
            case KEY_DC:
                //Check that we are not at eol()
                if(workingLine->can_go_right()) {
                    move(row, column+1);
                    workingLine->pop();
                }
                //if we are and we are not the last line, take the contents of the next line and remove the next line
                else if(workingLine != --doc.end()) {
                    //First copy it into previous line
                    previous = workingLine;
                    workingLine++;
                    previous->combine(*workingLine);
                    //Now, erase the line
                    doc.erase(workingLine);
                    workingLine = previous;
                }   
                break;
            default:
                if(column == maxcol-1) {
                    workingLine->insert(char(c));
                    workingLine->scroll_right();
                    move(row, column-1);
                }   
                else {
                    //Print character
                    workingLine->insert(char(c));
                    column += 1;
                }
                break;
        }
        clear();
        refill_from(starter);
        move(row, column);
    }
}

void pane::refill_from(int row) {
    //Create iterator to flow through list
    std::list<line>::iterator i = doc.begin();
    int counter = 1;
    preline numSys;

    //Get the maximum rows and columns of screen
    int maxrow, maxcol;
    getmaxyx(stdscr, maxrow, maxcol);

    //Print the header
    textHeader.print();

    //Print each line
    syntax_tuple startingSyntax; 
    while(i != doc.end()) {
        if(counter >= row && counter <= (maxrow + row - textHeader.size() - 1)) {
            numSys.print(counter);
            startingSyntax = i->print(ft, startingSyntax);
        }
        i++;
        counter++;
    }
    while(counter <= (maxrow + row - textHeader.size() - 1)) {
        numSys.print(counter);
        addch('\n');
        counter++;
    }
}

void pane::save(const string &filename) {
    //Creates iterator at first line of document
    std::list<line>::iterator iterator = doc.begin();
    //Opens text file and saves to it
    ofstream f;
    f.open(filename.c_str());
    if(f.is_open()) {
        //Save each line until we reach the end of doc list
        while(iterator != doc.end()) {
            f << iterator->getLine();
            iterator++;
        }
        f.close();
    }
    else
        endwin();
}
