/* ************************************************************************* *
 * main.c                                                                    *
 * ------                                                                    *
 *  Author:   David Johnson                                                  *
 *  Purpose:  This is a template for project 1.                              *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here.  For documentation of standard C library           *
 * functions, see the list at:                                               *
 *   http://pubs.opengroup.org/onlinepubs/009695399/functions/contents.html  *
 * ************************************************************************* */
 
#include <stdio.h>              	/* standard I/O */
#include <stdbool.h>            	/* bool types */
#include <stdint.h>             	/* uint32_t, uint8_t, and similar types */
#include <stdlib.h> 			/* malloc */
#include <inttypes.h>           	/* declares PRIu8 */

#include "../cmdline/parse.h"   	/* command line parser */
#include "debug.h"			/* DEBUG statements */
#include "../disasm/disasm.h"		/* Disassembler */
#include "../symbol/sym.h"		/* Symbols */
#include "../output/print-disasm.h"	/* Dissasembler Output */
#include "../interp/interp.h"		/* Interpreter */

/* ************************************************************************* *
 * Local function declarations                                               *
 * ************************************************************************* */
int file_open_and_read(const char *,uint8_t** array,int*); 
void print_decimal(uint8_t *array,int file_length);
int validate_instructions(instruction_t*, symtab_t*);

/* ************************************************************************* *
 * Purpose: Print out the given contents in decimal form.                    *
 * ************************************************************************* */
void print_decimal(uint8_t *array,int file_length)
{
    uint8_t index = 0;
    uint8_t width_counter = 0; /* make sure width never exceeds 80 characters */

    for (index = 0; index < file_length; index++)
    {
        printf("%03" PRIu8 " ", array[index]);
        width_counter += 4;
        if (width_counter >= 80)
        {
            width_counter = 0;
            printf("\n");
        }
    }
    printf("\n");
}

/* ************************************************************************* *
 * validate instrutions -- checks to make sure the instruction list is valid *
 *                                                                           *
 * Parameters                                                                *
 *   inst -- the list of instruction objects to check                        *
 *   symtab -- the table of symbols (if there is any) for the instructions   *
 *                                                                           *
 * Returns                                                                   *
 *    0 - if success                                                         *
 *    1 - if failure                                                         *
 * ************************************************************************* */
int validate_instructions(instruction_t* inst,symtab_t* symtab)
{
    //check for the lack of a STOP instruction
    bool stop_present = false;
    instruction_t* cur_inst = inst;
    while (cur_inst->next != NULL)
    {
	if (cur_inst->mnem == 0) //STOP
	    stop_present = true;
        cur_inst = cur_inst->next;
    }
    
    //check one more time after loop
    if (cur_inst->mnem == 0) //STOP
        stop_present = true;

    if (!stop_present) //no STOP instruction
    {
	printf("Your code does not contain a STOP instruction and therefore"
		" is invalid.  Exiting.\n");
	return 1;
    }

    return 0;
}

/* ************************************************************************* *
 * file_open_and_read -- opens and reads the file into an array              *
 *                                                                           *
 * Parameters                                                                *
 *   filename -- the name of the file to open to read                        *
 *   array -- the array to read all of the data into                         *
 *   file_length -- the number of elements in the file                       *
 *                                                                           *
 * Returns  								     *
 *    0 - if success 							     *
 *    1 - if failure 							     *
 * ************************************************************************* */
int file_open_and_read(const char *filename, uint8_t** array,int* file_length)
{
    FILE *fp;
    DEBUGx("Opening file \"%s\"\n",filename);
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("File \"%s\" does not exist\n",filename);
        return 1;
    }
    if (fseek(fp, 0, SEEK_END) != 0) /* check if seek is successful */
    {
        printf("Error with File\n");
        return 1;
    }
    *file_length = ftell(fp);
    if (*file_length == 0)
    {
        printf("Error File Empty\n");
        return 1;
    }

    DEBUGx("File contains %d bytes of data\n", *file_length);
    fseek(fp,0,SEEK_SET);
    *array = malloc(*file_length * sizeof(uint8_t));
    if (*array == NULL)
    {
        printf("Error No memory allocated");
        return 1;
    }

    fread(*array,sizeof(uint8_t),*file_length,fp);
    fclose(fp);
    return 0;
}

/* ************************************************************************* *
 * main -- main entry point into the program.                                *
 *                                                                           *
 * Parameters                                                                *
 *   argc -- the number of command-line arguments                            *
 *   argv -- the array of command-line arguments (array of pointers to char) *
 *                                                                           *
 * Returns                                                                   *
 *   Program exit status. The standard is to return 0 for normal exit (no    *
 *   errors occurred). If something unusual happens, return some number      *
 *   other than 0, which can then indicate to the user what happened.        *
 *                                                                           *
 * Notes                                                                     *
 *   This function is similar to the "public static void Main" method used   *
 *   in Java classes.                                                        *
 * ************************************************************************* */

int
main (int argc, char **argv)
{
    //create variables to pass by reference
    const char* filename = NULL;
    const char* symlist = NULL;
    _Bool interpret = false;
    
    //parse the command line.  Returns 1 if error
    if (parse_command_line (argc, argv,&filename,&symlist,&interpret) == 1)
	return 1;

    //create array to store the contents of file
    uint8_t *memory = NULL;
    int mem_length = 0;
    
    //open and read file.  Adjusts length of memory and contents of memory.
    //returns 1 (i.e. True) if error
    if (file_open_and_read(filename,&memory,&mem_length))
	return 1;

    //create symbol table to store symbols
    symtab_t* symtab = NULL;

    //check for symlist file and read it
    if (symlist != NULL)
    {
	//returns 1 (i.e. True) if error
        if (symlist_open_and_read(symlist,&symtab))
	    return 1;
    }

    //Create instruction to pass by reference
    instruction_t* instructions;
    //Determine the instructions in the array and create list of instructions
    determine_instructions(&instructions,memory,mem_length,&symtab);    
    
    //Make sure instructions are valid
    if (validate_instructions(instructions,symtab))
	return 1;

    //Print out the disassembler
    print_disassembler(instructions,memory,&symtab);

    if (interpret)
    {
	cpu_t pep8;
	interpret_memory(memory,&pep8,mem_length);
    }
	
    //free memory and set it to NULL before exiting
    free (memory);
    memory = NULL;
    return 0;
}
