//When compiling, make sure to include flag after the
//name of the file, otherwise g++ will not know to
//compile this file with ncurses

#include <iostream>
#include "brute.h"
using namespace std;

int main(int argc, char* argv[]) {
    //Checks input parameters
    if(argc > 2) {
        cerr << "Usage: " << argv[0] << " [FILENAME]" << endl;
        cerr << "Try '" << argv[0] << " --help' once I finish the --help function" << endl;
        return 1;
    }
    //Set filename to second argument passed to function, if it exists
    string fileName;
    if(argc == 2) 
        fileName = argv[1];

    try {
        //Creates window pane to open fileName
        pane mainScreen(fileName);

        //Initializes window to interact with
        mainScreen.initialize();

        //Gives mainScreen control of typing
        mainScreen.take_control();

        //Ends the window so we can use terminal
        endwin();
    }
    catch (err &e) {
        cerr << e.what() << endl;
        return 2;
    }

    return 0;
}
