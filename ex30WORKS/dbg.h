#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG     //lets you recompile your program so that all the debug msgs are removed
#define debug(M, ...)
#else             // if NDEBUG isnt defined,  DEBUG is packaged in a print macro which gives u file & line
#define debug(M, ...) fprintf(stderr, "DEBUG %s: %d: " M "\n", \
	__FILE__, __LINE__, ##__VA_ARGS__)
#endif
         //used in the other debugs to get a safe readable errno
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

         //log_err, log_warn, and log_info log messages for the end user
#define log_err(M, ...) fprintf(stderr, \
    "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, \
    clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, \
    "[WARN] (%s:%d: errno: %s) " M "\n", \
    __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n",\
    __FILE__, __LINE__, ##__VA_ARGS__)



//check - makes sure condition A is true, and if not, logs the error with 
//message M(with var args for log_err), then jumps to the functions error for cleanup

#define check(A, M, ...) if(!(A)) {\
    log_err(M, ##__VA_ARGS__); errno = 0; goto error; }

//senteniel - placed in any part of a function that shouldn't run, then jumps to error. 
//put this in if statements and switch statements (where I think Ive covered all the possible switches) to catch conditions that shouldnt happen
    //like default

#define sentinel(M, ...) { log_err(M, ##__VA_ARGS__); \
    errno = 0; goto error; }


//checkmem - makes sure a pointer is valid, and if it isn't, 
//reports it with 'out of memory'
    
#define check_mem(A) check((A), "Out Of Memory.")

//check_debug checks handles error but doesnt report if its a small common one
//checks it and cleans it up but doent print message if NDEBUG defined
#define check_debug(A, M, ...) if(!(A)) {debug(M, ##__VA_ARGS__);\
    errno = 0; goto error; }

#endif 
