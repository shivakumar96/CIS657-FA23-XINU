/**
 * @file     main.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <device.h>
#include <ether.h>
#include <platform.h>
#include <shell.h>
#include <stdio.h>
#include <thread.h>
#include <version.h>

static void print_os_info(void);

/* Start of kernel in memory (provided by linker)  */
extern void _start(void);
extern volatile uint32_t clkcountermsec;

void cpuproc(void);
void parasite(void);
void procio(void);

tid_typ our_shell_id ;

/**
 * Main thread.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.  The default is designed to do something reasonable
 * on all platforms based on the devices and features configured.
 */
thread main(void)
{

    //chprio(0, 3);
    //chprio(1, 3);

    kprintf("About to launch worker threads... %d\n",gettid());
    chprio(0, 0);

    /**
     * Benchmark testing for Xinu
     */

    int selectBenchmarkTest =3 ;

    // Variables used for benchmark testing
    tid_typ cpuProc1, cpuProc2, cpuProc3, cpuProc4; //stores cpurpoc process pids
    tid_typ ioPoc1, ioPoc2, ioPoc3, ioPoc4; //stores procio ids process pids
    tid_typ parasite1 ; //stores parasite process pid

    switch (selectBenchmarkTest) {
        /**
         *  Benchmark A:
         */
        case 1:

            kprintf("Executing Benchmark-A : 4 CPU process\n\n");

            // Creating CPU intensive process
            cpuProc1 = create(cpuproc, 1024, 1, "CPU-Proc-A", 0);
            cpuProc2 = create(cpuproc, 1024, 1, "CPU-Proc-B", 0);
            cpuProc3 = create(cpuproc, 1024, 1, "CPU-Proc-C", 0);
            cpuProc4 = create(cpuproc, 1024, 1, "CPU-Proc-D", 0);

            // resuming process
            resume(cpuProc1);
            resume(cpuProc2);
            resume(cpuProc3);
            resume(cpuProc4);

            break;
        /**
         *  Benchmark B:
         */
        case 2:

            printf("Executing Benchmark-B : 4 IO process\n\n");

            // Creating
            ioPoc1 = create(procio, 1024, 1, "IO-Proc-A", 0);
            ioPoc2 = create(procio, 1024, 1, "IO-Proc-B", 0);
            ioPoc3 = create(procio, 1024, 1, "IO-Proc-C", 0);
            ioPoc4 = create(procio, 1024, 1, "IO-Proc-D", 0);


            resume(ioPoc1);
            resume(ioPoc2);
            resume(ioPoc3);
            resume(ioPoc4);

            break;
        /**
         *  Benchmark C:
         */
        case 3:

            printf("Executing Benchmark-C : 4 CPU process and 4 IO Process\n\n");

            cpuProc1 = create(cpuproc, 1024, 1, "CPU-Proc-A", 0);
            cpuProc2 = create(cpuproc, 1024, 1, "CPU-Proc-B", 0);
            cpuProc3 = create(cpuproc, 1024, 1, "CPU-Proc-C", 0);
            cpuProc4 = create(cpuproc, 1024, 1, "CPU-Proc-D", 0);

            ioPoc1 = create(procio, 1024, 1, "IO-Proc-A", 0);
            ioPoc2 = create(procio, 1024, 1, "IO-Proc-B", 0);
            ioPoc3 = create(procio, 1024, 1, "IO-Proc-C", 0);
            ioPoc4 = create(procio, 1024, 1, "IO-Proc-D", 0);

            resume(cpuProc1);
            resume(cpuProc2);
            resume(cpuProc3);
            resume(cpuProc4);

            resume(ioPoc1);
            resume(ioPoc2);
            resume(ioPoc3);
            resume(ioPoc4);

            break;
        /**
         *  Benchmark D:
         */
        case 4:

            printf("Executing Benchmark-D : 3 CPU process and 1 Parasite\n\n");

            tid_typ cpuProc1 = create(cpuproc, 1024, 1, "CPU-Proc-A", 0);
            tid_typ cpuProc2 = create(cpuproc, 1024, 1, "CPU-Proc-B", 0);
            tid_typ cpuProc3 = create(cpuproc, 1024, 1, "CPU-Proc-C", 0);
            tid_typ parasite1 = create(parasite, 1024, 1, "Parasite-Proc", 0);

            resume(cpuProc1);
            resume(cpuProc2);
            resume(cpuProc3);
            resume(parasite1);

            break;
        /**
         * Invalid benchmark option selected
         */
        default:
            kprintf("Invalid benchmark test \n Exiting..\n");
    }




    recvclr();

    //our_shell_id =  create(shell, 4096, 50, "SHELL", 1, CONSOLE);
    //chprio(our_shell_id, 0);
    //resume(our_shell_id);




//    while (1) {
//        receive();
//        sleep(200);
//        fprintf(stdout, "\n\nMain Thread re-creating shell\n\n");
//        resume(create(shell, 4096, 20, "SHELL", 1, CONSOLE));
//    }

    return 0;
}

static void print_os_info(void)
{
    kprintf(VERSION);
    kprintf("\r\n\r\n");

#ifdef DETAIL
    /* Output detected platform. */
    kprintf("Processor identification: 0x%08X\r\n", cpuid);
    kprintf("Detected platform as: %s, %s\r\n\r\n",
            platform.family, platform.name);
#endif

    /* Output Xinu memory layout */
    kprintf("%10d bytes physical memory.\r\n",
            (ulong)platform.maxaddr - (ulong)platform.minaddr);
#ifdef DETAIL
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)platform.minaddr, (ulong)(platform.maxaddr - 1));
#endif


    kprintf("%10d bytes reserved system area.\r\n",
            (ulong)_start - (ulong)platform.minaddr);
#ifdef DETAIL
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)platform.minaddr, (ulong)_start - 1);
#endif

    kprintf("%10d bytes Xinu code.\r\n", (ulong)&_etext - (ulong)_start);
#ifdef DETAIL
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)_start, (ulong)&_end - 1);
#endif

    kprintf("%10d bytes stack space.\r\n", (ulong)memheap - (ulong)&_end);
#ifdef DETAIL
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)&_end, (ulong)memheap - 1);
#endif

    kprintf("%10d bytes heap space.\r\n",
            (ulong)platform.maxaddr - (ulong)memheap);
#ifdef DETAIL
    kprintf("           [0x%08X to 0x%08X]\r\n\r\n",
            (ulong)memheap, (ulong)platform.maxaddr - 1);
#endif
    kprintf("\r\n");
}
