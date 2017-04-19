#ifndef _WRAPPERS_H
#define _WRAPPERS_H
#include "filesystem.h"
int FS_write(char*, struct entry*);
int FS_setpos(struct entry*);
int FS_reset();
char FS_getc();
int FS_getint();
int FS_putint(unsigned int);
char FS_peek(unsigned int);
int FS_putc(unsigned char);
int FS_read();
int FS_jump(int);
int FS_getpos();
#endif
