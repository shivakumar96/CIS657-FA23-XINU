/**
 * @file kill.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <thread.h>
#include <stdint.h>

/**
 * @ingroup threads
 *
 * Kill a thread and remove it from the system
 * @param tid target thread
 * @return OK on success, SYSERR otherwise
 */
syscall avgresponse(tid_typ tid)
{
    register struct thrent *thrptr;     /* thread control block */

    // Check if thread ID is valid.
    if (isbadtid(tid)) {
        kprintf("Bad thread ID\n");
        return SYSERR;
    }

    thrptr = &thrtab[tid];

    if (thrptr->thrtotready == 0) {
        kprintf("Divide by zero error while attempting to calculate average response time for TID %d\n", tid);
        return SYSERR;
    }

    return (uint32_t) (thrptr->thrtotresp/thrptr->thrtotready);
}
