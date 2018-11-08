/*
  llzlab - luolongzhi algorithm lab 
  Copyright (C) 2013 luolongzhi 罗龙智 (Chengdu, China)

  This program is part of llzlab, all copyrights are reserved by luolongzhi. 

  filename: llz_parseopt.h 
  time    : 2012/07/09 23:38 
  author  : luolongzhi ( luolongzhi@gmail.com )
*/

#ifndef _LLZ_GETOPT_H
#define _LLZ_GETOPT_H

#include "llz_iir.h"

/*
    Below macro maybe defined in llz_print, if you do not want to use llz_print,
    you can define any other printf functions which you like.  Moreover, if you
    dont want print anything (for the effienece purpose of the program), you 
    can also define null, and the program will print nothing when running.
*/
#ifndef LLZ_PRINT 
//#define LLZ_PRINT(...)       
#define LLZ_PRINT       printf
#define LLZ_PRINT_ERR   LLZ_PRINT
#define LLZ_PRINT_DBG   LLZ_PRINT
#endif

#define FRAME_SIZE_MAX  8192

extern char  opt_inputfile[] ;
extern char  opt_outputfile[];

extern int   opt_framelen;

int llz_parseopt(int argc, char *argv[]);

#endif
