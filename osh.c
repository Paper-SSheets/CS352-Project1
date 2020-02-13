/*---------------------------------------------------------------------------
-					   COM S 352: Project 1 - Unix Shell                    -
-	Name: Steven Marshall Sheets                                            -
- 	Section: A-2                                                            -
-	NetID: smsheets@iastate.edu                                             -
-	Date: 2/5/2020                                                          -
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>

/*----------------------------------------------------------------------------
-	                            Prototypes                                   -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-	                                Notes                                    -
-----------------------------------------------------------------------------*/
// Compile with gcc lab07.c -o lab07 -lncurses
// Run with ./ds4rd.exe -d 054c:05c4 -D DS4_BT -g -b -j | ./lab07

/*---------------------------------------------------------------------------
-								Implementation                              -
----------------------------------------------------------------------------*/
#define MAX_LINE 80 /* The maximum length command */

int main(int argc, char *argv[]) {
    bool should_run = true; /* flag to determine when to exit program */
    char *prompt;
    int i, j = 0;

    while (should_run) {
        printf("osh>");
        fflush(stdout);
        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) parent will invoke wait() unless command included &
        */
        should_run = false; // tentative
    }
    return 0;
}


/**
 * PRE: dab
 *
 * POST: dab
 *
 * @param number - dab
 * @param current_mode - dab
 *
 */
void DELETE_THIS(int number, int current_mode) {
    // dab
}
