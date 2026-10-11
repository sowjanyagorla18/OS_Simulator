#include "ScisSos.h"

//Default scheduler 
SchedulerFunc scheduler_fn = sched_fcfs;
int scheduler_mode = SCHED_FCFS;

//Select the scheduler
void scissos_set_scheduler(SchedulerFunc fn, int mode)
{
    if (fn != NULL)
    {
        scheduler_fn = fn;
        scheduler_mode = mode;
    }
}

//1. First Come First Serve 
int sched_fcfs(int readyQ[], int qsize)
{
    if (qsize <= 0)
    {
        return EMPTY;
    }

    return readyQ[0];
}

//2. Priority Scheduling
   //Smaller priority_value means higher priority 
int sched_priority(int readyQ[], int qsize)
{
    int i;
    int selected_pid;

    if (qsize <= 0)
    {
        return EMPTY;
    }

    selected_pid = readyQ[0];

    for (i = 1; i < qsize; i++)
    {
        int pid = readyQ[i];

        if (pid < 0 || pid >= MAXPROC)
        {
            continue;
        }

        if (_proctable[pid] == NULL)
        {
            continue;
        }

        if (_proctable[selected_pid] != NULL &&
            _proctable[pid]->priority_value <
            _proctable[selected_pid]->priority_value)
        {
            selected_pid = pid;
        }
    }

    return selected_pid;
}

//3. Longest Job First
   //Select the process with the most remaining instructions 
int sched_ljf(int readyQ[], int qsize)
{
    int i;
    int selected_pid;
    int longest_remaining;

    if (qsize <= 0)
    {
        return EMPTY;
    }

    selected_pid = readyQ[0];

    if (selected_pid < 0 || selected_pid >= MAXPROC ||
        _proctable[selected_pid] == NULL)
    {
        return EMPTY;
    }

    longest_remaining =
        _proctable[selected_pid]->size -
        _proctable[selected_pid]->pc;

    for (i = 1; i < qsize; i++)
    {
        int pid = readyQ[i];
        int remaining;

        if (pid < 0 || pid >= MAXPROC ||
            _proctable[pid] == NULL)
        {
            continue;
        }

        remaining =
            _proctable[pid]->size -
            _proctable[pid]->pc;

        if (remaining > longest_remaining)
        {
            longest_remaining = remaining;
            selected_pid = pid;
        }
    }

    return selected_pid;
}
