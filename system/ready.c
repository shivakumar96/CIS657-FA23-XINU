/**
 * @file ready.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <thread.h>
#include <queue.h>

extern volatile uint32_t clkcountermsec;

/**
 * @ingroup threads
 *
 * Make a thread eligible for CPU service.
 * @param tid target thread
 * @param resch if RESCHED_YES, reschedules
 * @return OK if thread has been added to readylist, else SYSERR
 */
int ready(tid_typ tid, bool resch)
{
    register struct thrent *thrptr;

    if (isbadtid(tid))
    {
        return SYSERR;
    }

    thrptr = &thrtab[tid];

    //increment the thrtotready ony if the thread is created for the first time (THRSUSP) state or
    //it is moved from THCURR to THRREADY. This will be handled by resched function, but addding the condition for afer side.
    if(thrptr->state == THRSUSP || thrptr->state == THRCURR || thrptr->state == THRSLEEP){
        // Increment the total times the thread transitioned to the ready queue by 1.
        thrptr->thrtotready = thrptr->thrtotready + 1;
        thrptr->thrreadystart = clkcountermsec;
    }
    thrptr->state = THRREADY;



    insert(tid, readylist, thrptr->prio);

    if (resch == RESCHED_YES)
    {
        resched();
    }
    return OK;
}
