#include <clock.h>
#include <thread.h>
#include <stdint.h>
#include <stdio.h>

extern volatile uint32_t clkcountermsec;

void cpuproc(void){

    // Get the pointer to the current-running thread.
    struct thrent *thrptr = &thrtab[gettid()];

    int numLoops = 100;

    while (clkcountermsec < STOPCOND) {
        //kprintf("CPUbound at %d \n",clkcountermsec);
        //kprintf("clock is %d\n",clkcountermsec);
    }

    kprintf("Completed CPU Thread with PRIO: %d and name %s. Total CPU time: %d and average response time: %d total resp = %d toready = %d\n",
            thrptr->prio, thrptr->name, totcpu(gettid()),avgresponse(gettid()), thrptr->thrtotresp, thrptr->thrtotready);
    kprintf("------------------------------------------------------------------\n");
}
