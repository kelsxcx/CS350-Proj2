/* menu.c  
 * Menu display function for p2
 */
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include "menu.h"

void show_menu(void) {
    puts("\n========== CS 350 Project 2 - File and Directory Functions ==========\n"
		" ------------------- \n"
		" File Functions \n"
		" ------------------- \n"

			"  1. Display file information\n"
         "  2. Display a text file\n"
			"  3. Create an empty file\n"
         "  4. Copy a file \n"
			"  5. Delete a file or symbolic link\n"
         "  6. Change permissions\n"
	   "                     \n"
		" ------------------- \n"
		" Directory Functions \n"
		" ------------------- \n"
         "  7. Display current directory\n"
			"  8. Change directory\n"
         "  9. List directory contents\n"
         " 10. Rename or move an entry\n"
         " 11. Create a directory\n"
			" 12. Remove an empty directory\n"
	   "                     \n"
		" --------------------\n"
		" Link Functions \n"
		" --------------------\n"
			" 13. Create a hard or symbolic link\n"
         " 14. Display a symbolic link's target\n"
		" --------------------\n"
		"        EXIT  \n"
		" --------------------\n"
			"  0. Exit");
}

