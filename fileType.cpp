#include "fileType.h"
using namespace std;

syntax_tuple &syntax_tuple::operator=(const syntax_tuple &rhs) {
    first = rhs.first;
    last = rhs.last;
    color = rhs.color;
    max_length = rhs.max_length;
    multiline = rhs.multiline;
    override = rhs.override;
    return *this;
}

bool operator==(const syntax_tuple &lhs, const syntax_tuple &rhs) {
    if(lhs.first != rhs.first)
        return false;
    if(lhs.last != rhs.last)
        return false;

    return true;
}

bool operator!=(const syntax_tuple &lhs, const syntax_tuple &rhs) {
    return !(rhs == lhs);
}

string fileType::discover(const string &ext) {
    fstream f;
    string line;
    int index;
    const string indicator = "fileType:";
    
    f.open((path + "sfiles/types.txt").c_str());
    if(!f.is_open()) {
        return "";
    }
    else {
        while(!f.eof()) {
             //Read line from file
             getline(f, line);

             //Check if the line has our extension
             index = line.find(ext);
             if(index != string::npos) {
                 index = line.find(indicator);
                 index += indicator.size();

                 //The name of the file in which that extension is associated with is after the indicator
                 f.close();
                 return path + "sfiles/" + line.substr(index);
             }
        }
        //If we get to end of file without finding our extension, we return empty string
        f.close();
        return "";
    }
}           

fileType::fileType(string extension) {
    //Reading dictionary
    bool rd = false;
    //Reading syntax
    bool rs = false;

    fstream openedFile;
    string line;
    string temp, check;
    int index;
    syntax_tuple st;
    
    //Open our file to load the syntax highlighting
    string syntaxFile = discover(extension);
    openedFile.open(syntaxFile.c_str());

    //Load syntax highlighting
    if(openedFile.is_open()) {
        while(!openedFile.eof()) {
	    //Read each line
            getline(openedFile, line);
	    //When we reach an empty line, we are no longer reading a section, so we reset our booleans
            if(line[0] == ' ' || line.empty()) {
                rd = false;
                rs = false;
            }
	    //Get each word from the dictionary 
            if(rd) {
                dictionary.push_back(line);
            }
            //Read in syntax stuff
	    if(rs) {
	 	//Finds the opening symbol
                index = line.find(' ');
                st.first = line.substr(0, index);

		//Finds the closing symbol
                temp = line.substr(index+1);
                index = temp.find(' ');
		check = temp.substr(0, index);

		//If check is empty string (the user put an extra space), we set the last symbol as a space
		if(check == "")
		    st.last = " ";
		//If temp == "NONE", there is no delimiting syntax, so we keep st.last empty
		else if(check != "NONE")
                    st.last = check;
		else
		    st.last = "";

		//Finds the integer value for maximum length
		temp = temp.substr(index+1);
		index = temp.find(' ');
		check = temp.substr(0, index);
                if(check == "NONE")
		    st.max_length = -1;
		else
                    st.max_length = atoi(check.c_str());

		//Finds boolean value for if this syntax can span multiple lines
		temp = temp.substr(index+1);
		index = temp.find(' ');
		check = temp.substr(0, index);
		if(check == "TRUE")
		    st.multiline = true;
		else
		    st.multiline = false;

		//Finds boolean value if this syntax can be overridden by another syntax
		temp = temp.substr(index+1);
		index = temp.find(' ');
		check = temp.substr(0, index);
		if(check == "TRUE")
		    st.override = true;
		else
		    st.override = false;

		//Finds the color of the syntax
                temp = temp.substr(index+1);
                st.color = temp;
                
	  	syntax.push_back(st);
	    }

            //Make a substring that checks which section we're in
            index = line.find(' ');
            temp = line.substr(0, index);

            //Check which section we are in
            if(temp == "dict:")
                rd = true;
	    if(temp == "syntax:")
		rs = true;
        }
    openedFile.close();
    }
}

string findPath() {
    string user(getlogin());
    string p = "/home/" + user + "/bin/brute/";
    return p;
}
