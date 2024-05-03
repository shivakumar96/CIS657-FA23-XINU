/**
 * @file resched.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <thread.h>
#include <clock.h>
#include <queue.h>
#include <memory.h>
#include <stdint.h>

extern uint32_t cputime;
extern volatile uint32_t clkcountermsec;
extern volatile ulong clkticks ;

extern void ctxsw(void *, void *, uchar);
int resdefer;                   /* >0 if rescheduling deferred */


/**
 * @ingroup threads
 *
 * Reschedule processor to highest priority ready thread.
 * Upon entry, thrcurrent gives current thread id.
 * Threadtab[thrcurrent].pstate gives correct NEXT state
 * for current thread if other than THRREADY.
 * @return OK when the thread is context switched back
 */
int resched(void)
{
    uchar asid;                 /* address space identifier */
    struct thrent *throld;      /* old thread entry */
    struct thrent *thrnew;      /* new thread entry */

    if (resdefer > 0)
    {                           /* if deferred, increase count & return */
        resdefer++;
        return (OK);
    }

    throld = &thrtab[thrcurrent];

    throld->intmask = disable();
    if (THRCURR == throld->state)
    {

        // If the current thread has the highest priority when compared to the other threads in the
        // ready queue, we restore the context of the current thread and return. This means that the
        // state of this thread is still THRCURR.
        if (cputime == 0) {
            throld->thrtotcpu = throld->thrtotcpu + 1;
        } else {
            throld->thrtotcpu = throld->thrtotcpu + cputime;
        }

        // reset cputime.
        cputime = 0;
        //clkticks =0;

        if (nonempty(readylist) && (throld->prio > firstkey(readylist)))
        {
            restore(throld->intmask);
            clkticks =0;
            //kprintf("%s at %d \n",throld->name,clkcountermsec);
            return OK;
        }
        throld->state = THRREADY;
        insert(thrcurrent, readylist, throld->prio);

        // Increment the number of times this thread has transitioned to THRREADY.
        throld->thrtotready = throld->thrtotready + 1;
    }

//    kprintf(" Executing current thread %s at %d  clkticks is %d  prio is %d \n",
//            throld->name,clkcountermsec,clkticks,throld->prio);

    /* get highest priority thread from ready list */
    thrcurrent = dequeue(readylist);
    int old = thrcurrent;
    thrnew = &thrtab[thrcurrent];
    thrnew->state = THRCURR;

    asid = thrcurrent & 0xff;

    thrnew->thrtotresp = thrnew->thrtotresp + (clkcountermsec - thrnew->thrreadystart);

    ctxsw(&throld->stkptr, &thrnew->stkptr, asid);

    /* old thread returns here when resumed */
    restore(throld->intmask);

    return OK;
}
