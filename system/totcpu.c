/**
 * @file kill.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <thread.h>
#include <syscall.h>
#include <stdint.h>


extern uint32_t cputime;

/**
 * @ingroup threads
 *
 * Kill a thread and remove it from the system
 * @param tid target thread
 * @return OK on success, SYSERR otherwise
 */
syscall totcpu(tid_typ tid)
{
    register struct thrent *thrptr;     /* thread control block */

    // Check if thread ID is valid.
    if (isbadtid(tid)) {
        kprintf("Bad thread ID\n");
        return SYSERR;
    }

    thrptr = &thrtab[tid];

    // If queried thread is currently running, return thrtotcpu + cputime.
    if (thrptr->state == THRCURR) {
        return  thrptr->thrtotcpu + cputime;

    } else {
        kprintf(" not curr cpu is %d ",cputime);
        return  thrptr->thrtotcpu;
    }

    return OK;
}
