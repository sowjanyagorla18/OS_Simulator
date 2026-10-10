#include "ScisSos.h"
int main()
{
    ScisSosProcess *p;
    scissos_initialise();
    srand((unsigned int)time(NULL));
    p = scissos_proc_create("Process1",24000,PT_REG,MT_GOOD);
    p = scissos_proc_create("Process2",24000,PT_CMP,MT_GOOD);
    p = scissos_proc_create("Process3",24000,PT_IOE,MT_GOOD);
    p = scissos_proc_create("Process4",24000,PT_REG,MT_BAD);
    p = scissos_proc_create("Process5",24000,PT_CMP,MT_GOOD);
    p = scissos_proc_create("Process6",24000,PT_IOE,MT_BAD);
    p = scissos_proc_create("Process7",24000,PT_REG,MT_UGLY);
    p = scissos_proc_create("Process8",24000,PT_CMP,MT_GOOD);
    p = scissos_proc_create("Process9",24000,PT_IOE,MT_BAD);
    p = scissos_proc_create("Process10",24000,PT_REG,MT_GOOD);

    ScisSosPCreator(3);

    printf("\n");
    printf("==============================\n");
    printf("Starting Scheduler\n");
    printf("===============================\n");

    scissos_call_scheduler();

    scissos_print_timings();
    printf("\n");
    printf("All processes have finished.\n");

    return 0;
}