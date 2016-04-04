#ifndef __SYMBOLS__
#define __SYMBOLS__

/* Global constants defined in sym-const.c */
extern const char *SYMTYPES[];

/* Prototypes */
int symlist_open_and_read(const char *,symtab_t**);
int print_error_symtab(const char *,uint8_t);
symtype_t get_symtype_by_id(char *);
int letters_only(char *);
int numbers_only(char *);
void toUpperCase(char *);

#endif
