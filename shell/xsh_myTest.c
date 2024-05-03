/**
 * @file     xsh_test.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <thread.h>
//#include <syscall.h>

extern volatile uint32_t clkcountermsec;

void code(void) {
    struct thrent *thrptr = &thrtab[gettid()];

    int numLoops = 100;

    while ( clkcountermsec< STOPCOND) {
        //fprintf(stdout,"clock is %d\n",clkcountermsec);
    }

    fprintf(stdout,"Completed Thread with TID: %d and name %s. Total CPU time: %d and average response time: %d\n",
            gettid(), thrptr->name, totcpu(gettid()),avgresponse(gettid()));
    fprintf(stdout,"------------------------------------------------------------------\n");
}

/**
 * @ingroup shell
 *
 * Shell command (test) provides a mechanism for testing Xinu features.  The
 * action and output varies depending on the feature currently being tested.
 * This is not meant to serve as a permanent shell command for a particular
 * action.
 * @param nargs number of arguments
 * @param args  array of arguments
 * @return non-zero value on error
 */
shellcmd xsh_myTest(int nargs, char *args[])
{
    fprintf(stdout,"before calling cpuproc, tid: %d\n", gettid());
    cpuproc();
    return 0;
}
