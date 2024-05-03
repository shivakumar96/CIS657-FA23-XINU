#include <clock.h>
#include <thread.h>
#include <stdint.h>

extern volatile uint32_t clkcountermsec;
extern volatile ulong clkticks ;

void parasite(void){

    // Get the pointer to the current-running thread.
    struct thrent *thrptr = &thrtab[gettid()];

    int numLoops = 100;

    while (clkcountermsec < STOPCOND) {
        if  (clkticks+2>10) {
            sleep(0);
        }
    }

    kprintf("Completed Parasite Thread with PRIO: %d and name %s. Total CPU time: %d and average response time: %d\n",
            thrptr->prio, thrptr->name, totcpu(gettid()),avgresponse(gettid()));
    kprintf("------------------------------------------------------------------\n");
}
