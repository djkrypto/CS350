#ifndef __PARSE_COMMAND_LINE__
#define __PARSE_COMMAND_LINE__

/* ************************************************************************* *
 * The purpose of the #ifndef ... #define ... #endif structure is to prevent *
 * this code from being included multiple times caused by nested #include    *
 * statements. This comment can be removed.                                  *
 * ************************************************************************* */


/* ************************************************************************* *
 * parse.h                                                                   *
 * -------                                                                   *
 *  Author:   David Johnson                                                  *
 *  Purpose:  Header file for parse.c.                                       *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here. If none needed, delete this comment.               *
 * ************************************************************************* */


/* ************************************************************************* *
 * Function prototypes here. Note that variable names are often omitted.     *
 * ************************************************************************* */
int parse_command_line (int, char **,const char**,const char**,_Bool*);

#endif
