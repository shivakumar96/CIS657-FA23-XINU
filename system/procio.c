#include <clock.h>
#include <thread.h>
#include <stdint.h>

extern volatile uint32_t clkcountermsec;
extern volatile ulong clkticks ;

void procio(void){

    // Get the pointer to the current-running thread.
    struct thrent *thrptr = &thrtab[gettid()];

    int numLoops = 100;

    while (clkcountermsec < STOPCOND) {
        for (int i=0; i<numLoops; i++) {
            //kprintf("clock is %d\n",clkticks);
            sleep(50);
        }
    }

    kprintf("Completed IO Thread with PRIO: %d and name %s. Total CPU time: %d and average response time: %d, total resp %d toready = %d\n",
            thrptr->prio, thrptr->name, totcpu(gettid()),avgresponse(gettid()),thrptr->thrtotresp,thrptr->thrtotready);
    kprintf("------------------------------------------------------------------\n");
}
