Brute Force Text Editor

Description
-----------

Simple text editor made to run from the terminal. The 
interface is modelled after nano and is designed to be 
user friendly and logical to use. 


Documentation
-------------

The brute text editor requires the ncurses library.
To compile, use "make" while in working directory.

Once compiled use the command ./brute [FILENAME] to edit a file.
Commands:
	CNTL-Q -> Quit
	CNTL-S -> Save document
	Arrows to control cursor


Known Bugs:
-----------
-Does not implement all c++ keywords correctly. If there is a shorter one contained in a longer one,
	the editor gets confused. Examples: xor_eq, char32_t, or_eq...


Desired Features:
-----------------
-Indicator for when lines cannot be shown completely on the terminal 

-Redo/Undo

-Easily work on multiple files

-Easily add new programming languages to the editor
