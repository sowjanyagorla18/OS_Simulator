#include "ScisSos.h"

int main(int argc, char *argv[])
{
    ScisSosProcess *p;
    if (argc != 2){
        printf("Usage: %s <fcfs|priority|ljf>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "fcfs") == 0){
        scissos_set_scheduler(sched_fcfs, SCHED_FCFS);
    }
    else if (strcmp(argv[1], "priority") == 0){
        scissos_set_scheduler(sched_priority, SCHED_PRIORITY);
    }
    else if (strcmp(argv[1], "ljf") == 0){
        scissos_set_scheduler(sched_ljf, SCHED_LJF);
    }
    else{
        printf("Invalid algorithm. Use fcfs, priority, or ljf.\n");
        return 1;
    }

    scissos_initialise();
    srand((unsigned int)time(NULL));

    p = scissos_proc_create("Process1", 24000, PT_REG, MT_GOOD);
    if (p != NULL){
        p->_pcb->priority_value = 5;
    } 

    p = scissos_proc_create("Process2", 30000, PT_CMP, MT_GOOD);
    if (p != NULL){
        p->_pcb->priority_value = 3;
    }

    p = scissos_proc_create("Process3", 27000, PT_IOE, MT_GOOD);
    if (p != NULL){
        p->_pcb->priority_value = 8;
    }

    p = scissos_proc_create("Process4", 36000, PT_REG, MT_BAD);
    if (p != NULL){
        p->_pcb->priority_value = 1;
    }

    p = scissos_proc_create("Process5", 25000, PT_CMP, MT_GOOD);
    if (p != NULL){
        p->_pcb->priority_value = 6;
    }

    p = scissos_proc_create("Process6", 32000, PT_IOE, MT_BAD);
    if (p != NULL){
        p->_pcb->priority_value = 4;
    }

    p = scissos_proc_create("Process7", 40000, PT_REG, MT_UGLY);
    if (p != NULL){
        p->_pcb->priority_value = 9;
    }

    p = scissos_proc_create("Process8", 28000, PT_CMP, MT_GOOD);
    if (p != NULL){
        p->_pcb->priority_value = 2;
    }

    p = scissos_proc_create("Process9", 35000, PT_IOE, MT_BAD);
    if (p != NULL){
        p->_pcb->priority_value = 7;
    }

    p = scissos_proc_create("Process10", 24000, PT_REG, MT_GOOD);
    if (p != NULL){
        p->_pcb->priority_value = 10;
    }

    ScisSosPCreator(3);
    printf("==============================\n");
    printf("Starting Scheduler\n");
    printf("==============================\n");

    scissos_call_scheduler();
    scissos_print_timings();

    printf("\nAll processes have finished.\n");

    return 0;
}
