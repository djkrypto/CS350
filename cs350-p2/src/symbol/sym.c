/* ************************************************************************* *
 * sym.c                                                                     *
 * ------                                                                    *
 *  Author:   David Johnson                                                  *
 *  Purpose:  Perform all methods for symbols.                               *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here.  For documentation of standard C library           *
 * functions, see the list at:                                               *
 *   http://pubs.opengroup.org/onlinepubs/009695399/functions/contents.html  *
 * ************************************************************************* */

#include <stdio.h>              /* standard I/O */
#include <stdbool.h>            /* bool types */
#include <stdint.h>             /* uint32_t, uint8_t, and similar types */
#include <stdlib.h>             /* malloc */
#include <inttypes.h>           /* declares PRIu8 */
#include <string.h>             /* allows strtok */
#include <ctype.h>              /* allows "is" functions */

#include "../main/debug.h"      /* DEBUG statements */
#include "../disasm/disasm.h"   /* symtab_t */
#include "sym.h"		/* header file */
/* ************************************************************************* *
 * Local function declarations                                               *
 * ************************************************************************* */

/* ************************************************************************* *
 * Global variable declarations                                              *
 * ************************************************************************* */

/* ************************************************************************* *
 * Purpose: Convert a string to UpperCase                                    *
 * ************************************************************************* */
void toUpperCase(char *string)
{
    int index = 0;
    while (string[index] != '\0')
    {
        string[index] = toupper(string[index]);
        index++;
    }
}

/* ************************************************************************* *
 * letters_only - checks to make sure all characters in string are letters   *
 *                                                                           *
 * Parameters                                                                *
 *   string - the string to check for letters                                *
 *                                                                           *
 * Returns                                                                   *
 *    0 - if string consists of all letters                                  *                      
 *    1 - if string contains any non-letters                                 *
 * ************************************************************************* */
int letters_only(char *string)
{
    int len = strlen((const char*)string);
    int index = 0;
    for (index = 0; index < len; index++)
    {
        if ((islower(string[index]) == 0) && (isupper(string[index]) == 0))
            return 1;
    }
    return 0;
}

/* ************************************************************************* *
 * numbers_only - checks to make sure all characters in string are numbers   *
 *                                                                           *
 * Parameters                                                                *
 *   string - the string to check for numbers                                *
 *                                                                           *
 * Returns                                                                   *
 *    0 - if string consists of all numbers                                  *                      
 *    1 - if string contains any non-numbers                                 *
 * ************************************************************************* */
int numbers_only(char *string)
{
    int length = strlen(string);
    int index = 0;
    for (index=0; index<length; index++)
    {
        if (!isdigit(string[index]))
        {
            return 1;
        }
    }
    return 0;
}

/* ************************************************************************* *
 * Purpose: Convert a string code to a symtype_t.                            *
 * ************************************************************************* */
symtype_t get_symtype_by_id(char * code)
{
    uint32_t index = 0;
    toUpperCase(code);
    DEBUGx("type: %s\n",code);
    for (index = 0; index < NUMBER_OF_SYMTYPES; index++)
    {
        if (strncmp(code,SYMTYPES[index],5) == 0)
        {
            return (symtype_t)index;
        }
    }
    return INVALID_SYMTYPE_ID; /*is reached if code != to any mnemonic*/
}

/* ************************************************************************* *
 * Purpose: Print out an "illegally formatted symlist file" message          *
 * 	    Utilizes return statement to allow one-liners for errors in      *
 *	    symlist_open_and_read					     *
 *									     *
 * Returns: 1 in all cases since this is just an error message 		     *
 * ************************************************************************* */
int print_error_symtab(const char* filename,uint8_t error_code)
{
    printf("Symbol file \"%s\" is illegally formatted\n",filename);
    if (error_code == 1) //letters only
	printf("Your symbol type contained characters that were not letters\n");
    if (error_code == 2) //not one of the 4 symbol types
        printf("Your symbol type was not valid\n");
    if (error_code == 3) //not enough items on line
        printf("Each line of symbol table must contain at least 3 items\n");
    if (error_code == 4) //byte contains letters
        printf("The byte for one of the symbols is invalid\n");
    if (error_code == 5) //too many items on line
        printf("Each line of symbol table (excluding block symbols) can not"
	       " exceed 3 items\n");
    if (error_code == 6) //you didn't specify block length
        printf("Symbol is block but you didn't specify block length");
    return 1;
}

/* ************************************************************************* *
 * symlist_open_and_read -- opens and reads the symlist into an array        *
 *                                                                           *
 * Parameters                                                                *
 *   filename -- the name of the file to open to read                        *
 *   array -- An array of char* passed by reference                          *
 *   file_length -- the number of elements in the file                       *
 *                                                                           *
 * Returns                                                                   *
 *    0 - if success                                                         *
 *    1 - if failure                                                         *
 * ************************************************************************* */
int symlist_open_and_read(const char* filename,symtab_t** symtab)
{
    FILE *fp = fopen (filename, "r");
    if (fp == NULL)
    {
        printf("File \"%s\" does not exist\n",filename);
        return 1;
    }
    /* check if file is empty
    if (fseek(fp, 0, SEEK_END) != 0)
    {
        printf("Error with File\n");
        return 1;
    }
    if (ftell(fp) == 0)
    {
        printf("Error File Empty\n");
        return 1;
    }
    fseek(fp,0,SEEK_SET);
    */

    // fill linked list of symtabs for symbol table
    char buffer[50];
    char* token;
    const char delim[]=" \n\r\t";
    *symtab = malloc(sizeof(symtab_t));
    symtab_t *cur_symtab = *symtab; //current symtab

    while (fgets (buffer, 50, fp) != NULL)
    {
        token = strtok(buffer,delim);
        //checking if there is at least one item on this line
        if (token != NULL)
            cur_symtab->label = strdup(token);
        else //file contains an empty line, moving on to next line
            continue;
        token = strtok(NULL,delim);
        //checking if there are at least 2 items on this line
        if (token != NULL)
        {
            //does the symtype contain non-letters
            if (letters_only(token) == 1)
                return print_error_symtab(filename,1);
            cur_symtab->type = get_symtype_by_id(token);
            
	    //is the symtype invalid
            if (cur_symtab->type == INVALID_SYMTYPE_ID)
                return print_error_symtab(filename,2);
        }
        else
            return print_error_symtab(filename,3);
        token = strtok(NULL,delim);
        //checking if there are at least 3 items on this line
        if (token != NULL)
        {
            if (numbers_only(token))
                return print_error_symtab(filename,4);
            char* ptr; //used for below line and subsequently scrapped
            long int temp = strtol(token,&ptr,10);
            cur_symtab->offset = (off_t)temp;
        }
        else
            return print_error_symtab(filename,3);
        token = strtok(NULL,delim);
        
	//protect against more than 3 items in a single line except for .BLOCK
	if (token != NULL && cur_symtab->type == BLOCK)
            cur_symtab->block_length = (size_t)atoi(token);
        else if (token != NULL)
            return print_error_symtab(filename,5);
	else if (token == NULL && cur_symtab->type == BLOCK)
	    return print_error_symtab(filename,6);
	if (!feof(fp)) //returns nonzero if end-of-file has been reached on fp
        {
            cur_symtab->next = malloc(sizeof(symtab_t));
            cur_symtab = cur_symtab->next;
        }
    }
    cur_symtab->next = NULL; //set the final symtab next to NULL

    //workaround to fix the fact that you end up with extra symtab
    //checks if cur_symtab->next->next == NULL
    //Then sets cur_symtab->next == NULL to get rid of empty symtab
    cur_symtab = *symtab;
    while (cur_symtab->next->next != NULL)
	cur_symtab = cur_symtab->next;
    cur_symtab->next = NULL;

    fclose(fp);
    return 0;
}

