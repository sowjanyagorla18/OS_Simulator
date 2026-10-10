#include "ScisSos.h"

ScisSosPCB *_proctable[MAXPROC];
int _readyQ[MAXPROC];
int _blockQ[MAXPROC];
struct timeval _basetime;
TIMING pr_times[MAXPROC];

struct timeval *_diff_times(
    struct timeval a,
    struct timeval b)
{
    static struct timeval result;

    result.tv_sec = a.tv_sec - b.tv_sec;
    result.tv_usec = a.tv_usec - b.tv_usec;

    if (result.tv_usec < 0)
    {
        result.tv_sec--;
        result.tv_usec += 1000000;
    }

    return &result;
}

struct timeval *_add_times(
    struct timeval a,
    struct timeval b)
{
    static struct timeval result;

    result.tv_sec = a.tv_sec + b.tv_sec;
    result.tv_usec = a.tv_usec + b.tv_usec;

    if (result.tv_usec >= 1000000)
    {
        result.tv_sec++;
        result.tv_usec -= 1000000;
    }

    return &result;
}

struct timeval _max_times(
    struct timeval a,
    struct timeval b)
{
    if (a.tv_sec > b.tv_sec)
        return a;

    if (a.tv_sec < b.tv_sec)
        return b;

    if (a.tv_usec > b.tv_usec)
        return a;

    return b;
}

void scissos_initialise(void)
{
    int i;

    gettimeofday(&_basetime, NULL);

    for (i = 0; i < MAXPROC; i++)
    {
        _proctable[i] = NULL;
        _readyQ[i] = EMPTY;
        _blockQ[i] = EMPTY;

        pr_times[i].crt_time.tv_sec = 0;
        pr_times[i].crt_time.tv_usec = 0;

        pr_times[i].rsp_time.tv_sec = 0;
        pr_times[i].rsp_time.tv_usec = 0;

        pr_times[i].wt_time.tv_sec = 0;
        pr_times[i].wt_time.tv_usec = 0;

        pr_times[i].run_time.tv_sec = 0;
        pr_times[i].run_time.tv_usec = 0;
    }

    printf("OS Simulator initialized.\n");
}

static int pid_in_ready_queue(int pid, int qsize)
{
    int i;

    for (i = 0; i < qsize; i++)
    {
        if (_readyQ[i] == pid)
        {
            return 1;
        }
    }

    return 0;
}


static void update_ready_queue(int *qsize)
{
    int i;

    for (i = 0; i < MAXPROC; i++)
    {
        if (_proctable[i] == NULL)
        {
            continue;
        }

        if (_proctable[i]->ps_state != PS_RDY)
        {
            continue;
        }

        if (pid_in_ready_queue(i, *qsize))
        {
            continue;
        }

        if (*qsize >= MAXPROC)
        {
            return;
        }

        _readyQ[*qsize] = i;

        (*qsize)++;
    }
}

void scissos_call_scheduler(void)
{
    int qsize = 0;
    int i;
    int selectedPID;
    int pid;
    ScisSosPCB *pcb;

    for (i = 0; i < MAXPROC; i++)
    {
        _readyQ[i] = EMPTY;
    }

    update_ready_queue(&qsize);


    while (1)
    {

        update_ready_queue(&qsize);
        if (qsize == 0)
        {
            break;
        }
        selectedPID = SCHEDFUNC(_readyQ, qsize);

        if (selectedPID == EMPTY)
        {
            break;
        }
        pid = selectedPID;

        for (i = 0; i < qsize - 1; i++)
        {
            _readyQ[i] = _readyQ[i + 1];
        }

        qsize--;

        _readyQ[qsize] = EMPTY;

        if (_proctable[pid] == NULL)
        {
            continue;
        }

        pcb = _proctable[pid];


        pcb->ps_state = PS_RUN;

        printf("\nScheduler selected PID %d\n", pid);

        scissos_proc_run(pid);

        if (_proctable[pid] != NULL)
        {
            pcb = _proctable[pid];

            if (pcb->ps_state == PS_RDY)
            {
                if (qsize < MAXPROC)
                {
                    _readyQ[qsize] = pid;
                    qsize++;
                }
            }
        }
    }
}

void scissos_print_timings(void)
{
    int i;

    printf("\n");
    printf("============================================================\n");
    printf("PROCESS TIMINGS\n");
    printf("============================================================\n");

    printf("%-6s %-14s %-14s %-14s %-14s\n",
           "PID", "Creation", "Response", "Waiting", "Running");

    for (i = 0; i < MAXPROC; i++)
    {
        if (pr_times[i].crt_time.tv_sec == 0 &&
            pr_times[i].crt_time.tv_usec == 0)
        {
            continue;
        }

        printf("%-6d ", i);

        printf("%-14.6f ",
               (double)pr_times[i].crt_time.tv_sec +
               (double)pr_times[i].crt_time.tv_usec / 1000000.0);

        printf("%-14.6f ",
               (double)pr_times[i].rsp_time.tv_sec +
               (double)pr_times[i].rsp_time.tv_usec / 1000000.0);

        printf("%-14.6f ",
               (double)pr_times[i].wt_time.tv_sec +
               (double)pr_times[i].wt_time.tv_usec / 1000000.0);

        printf("%-14.6f\n",
               (double)pr_times[i].run_time.tv_sec +
               (double)pr_times[i].run_time.tv_usec / 1000000.0);
    }

    printf("============================================================\n");
}
