#ifndef FILETYPE_H
#define FILETYPE_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdlib.h>
using namespace std;

class syntax_tuple {
    public:
        // = operator
        syntax_tuple &operator=(const syntax_tuple &rhs);

        //Our variables
	string first;
	string last;
	string color;
	int max_length;
	bool multiline;
	bool override;
};


class fileType {
    public:
        //Constructor. Must pass filename.txt
        fileType(string);
        //Returns all keywords in this filetype
        vector<string> getDictionary() {return dictionary;}
        //Returns any syntax
        vector<syntax_tuple> getSyntax() {return syntax;}

    private:
        vector<string> dictionary;
        vector<syntax_tuple> syntax;
};

#endif
