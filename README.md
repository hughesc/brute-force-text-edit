Brute Force Text Editor

Description
-----------

Simple text editor made to run from the terminal. The 
interface is modelled after nano and is designed to be 
user friendly and logical to use. 


Documentation
-------------

The brute text editor requires the ncurses library.

To install, copy the folder into /home/user/bin/as the "brute" directory and use make. The text editor is compiled
into an executable called bts. Move this file directly into the /home/user/bin/ directory to use the editor.
I plan on making the installation process more automated in the future, but this works fine for now. 

Once compiled use the command bts [FILENAME] to edit a file.
Commands:
	CNTL-Q -> Quit
	CNTL-S -> Save document
	Arrows to control cursor

At the moment, the syntax highlighting feature only works for c++. To add additional languages, create
a text file with the following format:

dict:

keywords of the language (will be shown in bold)

syntax:

(start symbol) (end symbol) (maximum length-can be NONE) (whether the syntax can span multiple lines TRUE/
FALSE) (FALSE) (color written in caps)

An example of a syntax file can be found in brute/sfiles/c++.txt

Once you've created this file, use the command bts -s [FILENAME] [EXTENSIONS] to add the syntax file to the
directory to be used with the following extensions. Use double quotes if multiple extensions are to be 
assigned. The text editor creates a copy of the syntax file and stores it in its sfiles directory, so there
is no need to keep the original. It then creates a link to the file to be used when needed.

To list all syntax files, use bts -l. To remove one of these files, use bts -r [FILENAME]. 

Known Bugs:
-----------
-Does not implement all c++ keywords correctly. If there is a shorter one contained in a longer one,
	the editor gets confused. Examples: xor_eq, char32_t, or_eq...

-No error checking when adding a syntax highlighting file. Links can be added without the actual file.

Desired Features for Future Versions:
-----------------
-Redo/Undo

-Cut/Paste
