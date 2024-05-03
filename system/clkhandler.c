/**
 * @file     clkhandler.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <stddef.h>
#include <queue.h>
#include <clock.h>
#include <thread.h>
#include <platform.h>
#include <stdint.h>

#if RTCLOCK

void wakeup(void);
int resched(void);

extern volatile uint32_t clkcountermsec;
extern volatile uint32_t cputime;

/**
 * @ingroup timer
 *
 * Interrupt handler function for the timer interrupt.  This schedules a new
 * timer interrupt to occur at some point in the future, then updates ::clktime
 * and ::clkticks, then wakes sleeping threads if there are any, otherwise
 * reschedules the processor.
 */
interrupt clkhandler(void)
{

    // this function is called every msec.
    // Update the clock every ms.
    clkupdate(platform.clkfreq / CLKTICKS_PER_SEC);

    /* Another clock tick passes. */
    clkticks++;

    // Update the clock counter.
    clkcountermsec++;

    // increment the CPU time.
    cputime++;

    ////kprintf("clkcountermsec value TID %d at clkhandler: %d and cpu at %d\n", thrcurrent, clkcountermsec,cputime);
//    if (gettid() > 2) {
//        kprintf("clkcountermsec value from clkhandler: %d\n", clkcountermsec);
//    }

    struct thrent *thrptr = &thrtab[gettid()];

    int mask = 0;
    switch(thrptr->prio) {
        // 0: Xinu's idle thread.
        case 0: clkticks >= QUANTUMIDLE;break;
        // 1: CPU-intesive
        case 1: mask = clkticks >= QUANTUMCPU; break;
        // 2: IO-intensive
        case 2: mask = clkticks >= QUANTUMIO; break;
        case 3: mask = 1; break;
    }

    if (mask) {
        clktime++;
        clkticks = 0;
    }

    /* Update global second counter. */
    /*
     * CLKTICKS_PER_SEC is
     * */

//    if (CLKTICKS_PER_SEC == clkticks)
//    {
//        clktime++;
//        clkticks = 0;
//    }

    /* If sleepq is not empty, decrement first key.   */
    /* If key reaches zero, call wakeup.              */
    if (nonempty(sleepq) && (--firstkey(sleepq) <= 0))
    {
        wakeup();
    }
    else
    {
        resched();
    }
}

#endif /* RTCLOCK */
