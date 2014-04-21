#include "brute.h"
using namespace std;

line::line(int t): tab(0), cur(0), start(0), type(BLACK) {
    //Indents as much as needed
    for(int i=0; i<tab; i++) {
        insert(' ');
    }
}

line::line(string c): tab(0), characters(c), cur(0), start(0) {
    type = BLACK;
}

line::line(string c, int t): tab(t), characters(c), cur(0), start(0) {
    //Indents as much as needed
    for(int i=0; i<tab; i++) {
        characters.insert(characters.begin(), ' ');
    }
    type = BLACK;
}

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

//Helper function for the print function
vector<bool> findBold(const string &str, const vector<string> &dictionary) {
    //The vector we will be returning. We initialize it as false
    vector<bool> rtn(str.size(), false);
    //Loop through the dictionary, setting our location to the beginning of the line each time
    for(int i=0; i<dictionary.size(); i++) {
        int location = 0;
        while(true) {
            //Find keywords in the dictionary in our line
            location = str.find(dictionary[i], location);
            //If we find a keyword, we return the vector true at the beginning of the word
            if(location != string::npos) {
                if((location == 0 || !isalpha(str[location-1])) && (location == str.size()-1 || !isalpha(str[location+dictionary[i].size()]))) {
                    rtn[location] = true;
                    //And set it true at the end of the word
                    rtn[location+dictionary[i].size()] = true;
                    
                }
                location++;
            }
            //If we don't find that keyword, we go to the next one
            else 
                break;
        }
    }
    return rtn;
}

int str_color_pair(string str) {
    int index = str.find(' ');
    string color;

    //Remove any whitespace from the back of the string
    if(index == string::npos)
	color = str;
    else
	color = str.substr(0, index);

    for(int i=0; i<color.size(); i++) {
        toupper(color[i]);
    }

    if(color == "BLUE") {
	return BLUE;
    }
    else if(color == "RED")
	return RED;
    else if(color == "GREEN")
	return GREEN;
    else {
	return BLACK;
    }
}

bool isDefault(syntax_tuple s) {
    if(s.first != "")
	return false;
    if(s.last != "")
	return false;
    return true;
}
	
//Helper function for print function
vector<syntax_tuple> findSyntax(const string &str, const vector<syntax_tuple> &syntax, syntax_tuple startingSyntax) {
    vector<syntax_tuple> syntaxAt;
    int i, j;
    int begin_syntax = str.size();
    syntax_tuple defSyntax;

    //Initialize syntaxAt vector as long as str string with each position override-able
    for(i=0; i<str.size(); i++) {
        defSyntax.override = true;
        syntaxAt.push_back(defSyntax);
    }

    //If our terminal doesn't support colors, theres no reason to continue
    if(!has_colors()) {
        return syntaxAt;
    }

    //Check for syntax
    int start = 0;

    while(start < str.size()) {
        syntax_tuple syntaxAtStart = defSyntax;
	int first = str.size();

        //Use the default syntax passed to the function
        if(!isDefault(startingSyntax) && start == 0) {
	    first = 0;
	    syntaxAtStart = startingSyntax;
        }
	
        //Find syntax that appears first in the string
        for(i=0; i<syntax.size(); i++) {
	    int syntaxBegin = str.find(syntax[i].first, start);
	    if(syntaxBegin < first && syntaxBegin != string::npos) {
	        first = syntaxBegin;
	        syntaxAtStart = syntax[i];
	    }
        }
	start = first;

        //Update our syntax vector
        int end = str.find(syntaxAtStart.last, start+1);
	if(end == string::npos || syntaxAtStart.last == "") {
	    end = str.size();
	}
	else {
	    end += syntaxAtStart.last.size();
	}
        while(start < end) {
            syntaxAt[start] = syntaxAtStart;
	    start++;
        }
    }
    return syntaxAt;
}
            
//Prints line without preline
syntax_tuple line::print(fileType f, syntax_tuple startingSyntax) {
    int maxrow, maxcol;
    getmaxyx(stdscr, maxrow, maxcol);
    int length = maxcol - PRELINE_SIZE - strlen(PRELINE_DELIMETER);
    
    string sub = characters.substr(start, length);

    //Embolden any keywords that we find
    //Togglebold vector tells us the location of beginning and ends of words
    vector<bool> toggleBold = findBold(characters, f.getDictionary());
    vector<syntax_tuple> syntaxAt = findSyntax(characters, f.getSyntax(), startingSyntax);
    
    //Make the vector with the color in each spot from our syntax vector
    vector<int> colorVector;
    for(int k=0; k<syntaxAt.size(); k++) {
        colorVector.push_back(str_color_pair(syntaxAt[k].color));
    }
    
    bool embolden;
    bool multiLine = false;
    int count = 0;

    //Determine starting state of embolden, depending on first word of line
    for(int i=0; i<toggleBold.size(); i++) {
        if(toggleBold[i] && (i < start)) {
            count++;
        }
    }
    
    if(count % 2) {
        embolden = true;
        attron(A_BOLD);
    }
    else
        embolden = false;

    //Print sub-line shown on screen
    int j;
    for(j=0; j<sub.size(); j++) {
        if(toggleBold[j+start] == true && isDefault(syntaxAt[j+start])) {
            if(embolden) {
                attroff(A_BOLD);
                embolden = false;
            }
            else {
                attron(A_BOLD);
                embolden = true;
            }
        }
	//Turn on colors if we found we needed any
        attron(COLOR_PAIR(colorVector[j+start]));

        addch(sub[j]);
    }                  
    attroff(A_BOLD);
    attroff(COLOR_PAIR(colorVector[j+start-1]));

    if(syntaxAt[j+start-1].multiline == true)
        return syntaxAt[syntaxAt.size()-1];
    else {
	syntax_tuple def;
	return def;
    } 
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
    //Previous way of finding iterator. Will keep it just in case
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

int line::cursorPos() {
    return cur.position();
}

int line::tabLevel() {
    tab = 0;
    for(int i=0; i<characters.size(); i++) {
        if(characters[i] == '\t' || characters[i] == ' ')
            tab++;
        else
            break;
    }
    return tab;
}

int line::size() const{
    return characters.size() + PRELINE_SIZE + strlen(PRELINE_DELIMETER);
}  

inline bool operator==(const line& lhs, const line& rhs) {
    return lhs.getLine() == rhs.getLine();
}

inline bool operator!=(const line& lhs, const line& rhs) {
    return !(lhs == rhs);
}
    
    
    

