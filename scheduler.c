#include "ScisSos.h"
int sched_fcfs(int readyQ[], int qsize)
{
    if (qsize <= 0)
    {
        return EMPTY;
    }

    return readyQ[0];
}