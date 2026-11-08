/* p2.c 
 * POSIX.1-2008 menu-based file and directory utility.
 * Paths are literal: spaces work; quotes, ~, wildcards and variables are not expanded.
 * Copy should refuse existing destinations. 
 * Rename should use POSIX replacement semantics after confirmation. 
 */
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include "menu.h"
#include "directory.h"
#include "file.h"
#include "utility.h"


int main(void) {
	while (true) {

		/* Display menu and obtain valid user selection */
		show_menu();
		char *choice = ask("Choice: "), *end;
		errno = 0;
		long option = strtol(choice, &end, 10);
		int valid = ( (errno == 0) && (end != choice) && (*end == '\0') && 
						  (option >= 0) && (option <= 14));
		free(choice);

		if (!valid) { 
			puts("Enter a number from 0 through 14."); 
			continue; 
		}

		/* process options 0 (exit) and 7 (which doesn't need a path argument */
		if (option == 0)
			return EXIT_SUCCESS;
		if (option == 7) {
			putchar('\n');
			current_directory(); 
			press_enter();
			continue;
		}

		/* for other options, solicit the path argument */
		putchar('\n');
		char *path = ask(option == 9 ? "Directory (blank = current): " : "Filename/path: ");
		if (!*path && option != 9) { 
		puts("A pathname is required."); 
		free(path); 
		continue; 
		}

		switch (option) {
		/* cases are largely self-explanatory */
		case 1:
			file_information(path); 
			press_enter();
			break;
		case 2: 
			display_file(path); 
			press_enter();
			break;
		case 3: 
			create_empty(path);
			press_enter();
			break;
		/* A directory IS a file, so rename works the same way for both 
		 * a regular file and a directory file.  We can place two labels
		 * on the same switch block, as seen below.
		 */
		case 4: 
			{
			char *dest = ask("Destination pathname: ");
			if (!*dest) 
				puts("A destination is required.");
			else
				copy_file(path, dest);
			free(dest); 
			press_enter();
			break;
			}
		case 10:
			{ 
			/* 
			 * We need brackets here because a switch case can't start with a declaration.
			 * Adding the brackets turns into a compound statement, which is acceptable.
			 */
			/* solicit the second argument */
         char *dest = ask("Destination pathname: ");
         if (!*dest) 
				puts("A destination is required.");
			rename_entry(path, dest);
			free(dest); 
			press_enter();
			break;
        }
		case 5:
			delete_entry(path);
			press_enter();
			break;
		case 6: 
			change_permissions(path); 
			press_enter();
			break;
		/* option 7 was handled before the switch */
		case 8: 
			change_directory(path);
			press_enter();
			break;
		case 9:
			list_directory(path); 
			press_enter();
			break;
		case 11: 
			create_directory(path);
			press_enter();
			break;
		case 12:
			remove_directory(path);
			press_enter();
			break;
		case 13: 
			/*
			 * Again, we start the block with a declaration and so need the 
			 * brackets to turn this into an acceptable type of statement.
			 * A compound statement is such a statement.
			 */
			{
			/* solicit the additional arguments needed */
			char *kind = ask("Link type: h = hard, s = symbolic: ");
			if (strcmp(kind, "h") && strcmp(kind, "s")) 
				puts("Choose h or s.");
			else {
				char *dest = ask("New link pathname: ");
				if (!*dest) 
					puts("A link pathname is required.");
				else {
					make_link(kind, path, dest);
				}
           	free(dest);
			}
        	free(kind); 
			press_enter();
			break;
        }
		case 14: 
			show_link(path); 
			press_enter();
			break;
		default: 
			break;
		} //end switch
	free(path);
	} //end while 
}

