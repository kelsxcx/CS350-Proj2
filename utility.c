/* utility.c
 * utility functions for p2 
 */
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#include "utility.h"

/* 
 * displays the given prompt
 * obtains user input
 * performs appropriate error_checking
 */
char *ask(const char *prompt) {
	char *s = NULL;
	size_t cap = 0;
	fputs(prompt, stdout);
	fflush(stdout);
	ssize_t n = getline(&s, &cap, stdin);

	// input error_checking
	if (n < 0) {
		free(s);
		if (ferror(stdin)) { 
			perror("input"); 
			exit(EXIT_FAILURE); 
		}
		putchar('\n');
		exit(EXIT_SUCCESS);
	}

	if (n && (s[n-1] == '\n')) 
		s[--n] = '\0';

	if (strlen(s) != (size_t)n) {
		free(s);
		fputs("Input contains a NUL byte.\n", stderr);
		exit(EXIT_FAILURE);
	}

	return s;
}

/*
 * return true if user enters y or Y
 * else false
 */
int confirm(const char *prompt) {
	char *s = ask(prompt);
	int yes = (strcmp(s, "y") == 0) || (strcmp(s, "Y") == 0);
	free(s);
 	return yes;
}

/*
 * pause to let the user view the output
 * before displaying the menu again
 */
void press_enter(void) {
	printf("Press [ENTER] to continue... ");
	fflush(stdout); // Force text out of the buffer immediately

	// Clear out any residual newline characters left over from previous inputs
	int ch;
	while (((ch = getchar()) != '\n') && (ch != EOF));
}

