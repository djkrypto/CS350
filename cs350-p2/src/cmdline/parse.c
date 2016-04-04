/* ************************************************************************* *
 * parse.c                                                                   *
 * -------                                                                   *
 *  Author:   David Johnson                                                  *
 *  Purpose:  This code is used to parse the command-line arguments for      *
 *            CS 350 project 1.                                              *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here.                                                    *
 * ************************************************************************* */

#include <stdio.h>              /* standard I/O */
#include <unistd.h>             /* declares getopt() */
#include <ctype.h>              /* declares isprint() */
#include <stdbool.h>		/* bool type */

#include "parse.h"              /* prototypes for exported functions */
#include "../main/debug.h"      /* DEBUG statements */
/* ************************************************************************* *
 * Local function prototypes                                                 *
 * ************************************************************************* */


/* ************************************************************************* *
 * Global variable declarations                                              *
 * ************************************************************************* */


/* ************************************************************************* *
 * print_error -- prints the appropriate error message for illegal flags     *
 * Notes                                                                     *
 *   This function is designed to eliminate the repetition of print messages *
 *   for illegal combinations of flags from the user.                        *
 * ************************************************************************* */
void print_error()
{
    printf("You entered an illegal combination of options\n");
}


/* ************************************************************************* *
 * parse_command_line -- parses the command-line arguments using getopt()    *
 *                                                                           *
 * Parameters                                                                *
 *   argc -- the number of command-line arguments                            *
 *   argv -- the array of command-line arguments (array of pointers to char) *
 *                                                                           *
 * Returns                                                                   *
 *   Parsing success status. If the command-line arguments are successfully  *
 *   parsed with no errors, we return the value 0. If any errors occur (such *
 *   as passing an invalid flag or passing a file that does not exist), we   *
 *   return 1 back to the calling function and stop processing               *
 *                                                                           *
 * Notes                                                                     *
 *   This function is designed to be extensible for multiple command-line    *
 *   options. In C, though, you can only return a single value. We get       *
 *   around this restriction by using call-by-reference parameters. For      *
 *   instance, if we want to set a boolean value, the parameter list would   *
 *   be modified as:                                                         *
 *          
 *     parse_command_line (..., bool * boolval)                              *
 *                                                                           *
 *   Then, the function would be called from somewhere else like:            *
 *                                                                           *
 *     bool my_bool_value;                                                   *
 *     int return_code;                                                      *
 *     return_code = parse_command_line (..., &my_bool_value);               *
 * ************************************************************************* */

int
parse_command_line (int argc, char **argv,const char **filename,const char **svalue,
		    _Bool* interpret)
{
    int sflag = 0;
    opterr = 0;
  
    int option;
    while ((option = getopt (argc, argv, "s:i")) != -1)
    {
        switch (option)
        {
	case 's':
	    sflag++;
	    *svalue = optarg;
	    break;
	case 'i':
	    *interpret = true;
	    break;
	case '?':
            if (isprint (optopt))
            {
                print_error();
                return 1;
            }
            else
            {
               	print_error();
                return 1;
            }
            break;
        default:
	    print_error();
            return 1;
	}
    }

    if (argc > optind)
    {
        *filename = argv[optind];
        optind++;
        if (*filename == NULL)
        {
            printf("Filename is invalid. Please try again.");
            return 1;
        }
        if (argc > optind)
        {
            printf("Additional arguments after %s will be ignored.\n",*filename);
        }
    }
    else
    {
        printf("Filename is a required argument\n");
        return 1;
    }

    return 0;
}
