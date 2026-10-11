#include "ScisSos.h"

static int creator_remaining = 0;
static int creator_pid = EMPTY;

static int get_free_pid()
{
    int i;
    for (i = 1; i < MAXPROC; i++)
    {
        if (_proctable[i] == NULL)
        {
            return i;
        }
    }
    return EMPTY;
}

static double get_long_call_probability(int process_type)
{
    if (process_type == PT_REG)
    {
        return REG_THR;
    }

    if (process_type == PT_CMP)
    {
        return CMP_THR;
    }

    if (process_type == PT_IOE)
    {
        return IOE_THR;
    }

    return REG_THR;
}

static void generate_code(ScisSosInst **code,int size,int process_type) {
    int i;
    double probability;
    probability = get_long_call_probability(process_type);

    for (i = 0; i < size; i++)
    {
        code[i]->_inum = i;
        double r = (double)rand() / (double)RAND_MAX;

        if (r < probability)
        {
            code[i]->_syscall = 1;
        }
        else
        {
            code[i]->_syscall = 0;
        }
        code[i]->_addref = rand() % 100000;
    }
}

ScisSosProcess *scissos_proc_create(char *name,int process_size,int process_type,int memory_type) {
    int pid;
    int i;
    ScisSosProcess *process;
    ScisSosPCB *pcb;

    if (process_size < 24000)
    {
        process_size = 24000;
    }
    pid = get_free_pid();
    if (pid == EMPTY)
    {
        printf("ERROR: Process table is full.\n");
        return NULL;
    }
    process = malloc(sizeof(ScisSosProcess));

    if (process == NULL)
    {
        printf("ERROR: Cannot allocate process.\n");
        return NULL;
    }

    pcb = malloc(sizeof(ScisSosPCB));

    if (pcb == NULL)
    {
        free(process);
        printf("ERROR: Cannot allocate PCB.\n");
        return NULL;
    }
    strncpy(
        process->_pname,
        name,
        MAXPROCNAME - 1
    );

    process->_pname[MAXPROCNAME - 1] = '\0';
    process->_PID = pid;
    process->_psize = process_size;

    process->_CODE =malloc(sizeof(ScisSosInst *) * process_size);

    if (process->_CODE == NULL)
    {
        free(pcb);
        free(process);

        printf("ERROR: Cannot allocate code.\n");

        return NULL;
    }

    for (i = 0; i < process_size; i++)
    {
        process->_CODE[i] = malloc(sizeof(ScisSosInst));

        if (process->_CODE[i] == NULL)
        {
            int j;
            for (j = 0; j < i; j++)
            {
                free(process->_CODE[j]);
            }
            free(process->_CODE);
            free(pcb);
            free(process);

            printf(
                "ERROR: Cannot allocate instruction.\n"
            );

            return NULL;
        }
    }

    generate_code(process->_CODE,process_size, process_type);

    pcb->pid = pid;
    pcb->uid = 1 + rand() % MAXUSRS;
    pcb->size = process_size;
    pcb->priority_value = DEFPRIO;
    pcb->ps_state = PS_RDY;
    pcb->p_type = process_type;
    pcb->m_type = memory_type;
    pcb->pc = 0;
    pcb->p_code = process->_CODE;
    pcb->p_timeslice = DEFTS;

    for (i = 0; i < MAXPGES; i++)
    {
        pcb->pg_table[i][0] = EMPTY;
        pcb->pg_table[i][1] = EMPTY;
    }

    process->_pcb = pcb;
    _proctable[pid] = pcb;

    gettimeofday(&pr_times[pid].crt_time,NULL);{
        struct timeval now;
        gettimeofday(&now, NULL);
        pr_times[pid].crt_time.tv_sec =
            now.tv_sec - _basetime.tv_sec;

        pr_times[pid].crt_time.tv_usec =
            now.tv_usec - _basetime.tv_usec;

        if (pr_times[pid].crt_time.tv_usec < 0)
        {
            pr_times[pid].crt_time.tv_sec--;

            pr_times[pid].crt_time.tv_usec +=
                1000000;
        }
    }
    pr_times[pid].rsp_time.tv_sec = 0;
    pr_times[pid].rsp_time.tv_usec = 0;

    pr_times[pid].wt_time.tv_sec = 0;
    pr_times[pid].wt_time.tv_usec = 0;

    pr_times[pid].run_time.tv_sec = 0;
    pr_times[pid].run_time.tv_usec = 0;

    return process;
}

static void delete_process(int pid)
{
    int i;

    ScisSosPCB *pcb;


    if (pid < 0 || pid >= MAXPROC)
    {
        return;
    }


    pcb = _proctable[pid];

    if (pcb == NULL)
    {
        return;
    }

    if (pcb->p_code != NULL)
    {
        for (i = 0; i < pcb->size; i++)
        {
            free(pcb->p_code[i]);
        }

        free(pcb->p_code);
    }

    _proctable[pid] = NULL;

    free(pcb);
}


int scissos_proc_run(int pid)
{
    ScisSosPCB *pcb;
    int executed;
    struct timeval start_time;
    struct timeval end_time;
    struct timeval *run_diff;
    struct timeval now;
    struct timeval *elapsed;
    struct timeval *wait_time;

    if (pid < 0 || pid >= MAXPROC)
    {
        return -1;
    }

    pcb = _proctable[pid];

    if (pcb == NULL)
    {
        return -1;
    }

    if (pcb->pc == 0 && pr_times[pid].rsp_time.tv_sec == 0 && pr_times[pid].rsp_time.tv_usec == 0)
    {
        gettimeofday(&now, NULL);

        now.tv_sec -= _basetime.tv_sec;
        now.tv_usec -= _basetime.tv_usec;

        if (now.tv_usec < 0)
        {
            now.tv_sec--;
            now.tv_usec += 1000000;
        }

        elapsed =
            _diff_times(
                now,
                pr_times[pid].crt_time
            );

        pr_times[pid].rsp_time =
            *elapsed;
    }

    if (pid == creator_pid &&
        creator_remaining > 0)
    {
        char name[MAXPROCNAME];

        sprintf(
            name,
            "Child_%d",
            creator_remaining
        );

        scissos_proc_create(
            name,
            24000,
            PT_REG,
            MT_GOOD
        );
        creator_remaining--;


        if (creator_remaining == 0)
        {
            pcb->pc = pcb->size;
            pcb->ps_state = PS_DEAD;
            gettimeofday(&now, NULL);
            now.tv_sec -= _basetime.tv_sec;
            now.tv_usec -= _basetime.tv_usec;
            if (now.tv_usec < 0)
            {
                now.tv_sec--;
                now.tv_usec += 1000000;
            }

            elapsed =
                _diff_times(
                    now,
                    pr_times[pid].crt_time
                );

            wait_time =
                _diff_times(
                    *elapsed,
                    pr_times[pid].run_time
                );

            if (wait_time->tv_sec < 0)
            {
                wait_time->tv_sec = 0;
                wait_time->tv_usec = 0;
            }

            pr_times[pid].wt_time =
                *wait_time;

            delete_process(pid);

            return 0;
        }
        pcb->ps_state = PS_RDY;
        return 0;
    }


    gettimeofday(&start_time,NULL);
    executed = 0;

    while (pcb->pc < pcb->size && executed < pcb->p_timeslice)
    {
        ScisSosInst *instruction;

        instruction =
            pcb->p_code[pcb->pc];
        pcb->pc++;
        executed++;

        if (instruction->_syscall == 1)
        {
            break;
        }
    }
    gettimeofday(&end_time,NULL);

    run_diff = _diff_times( end_time,start_time);
    pr_times[pid].run_time =*_add_times(pr_times[pid].run_time,*run_diff);



    if (pcb->pc >= pcb->size)
    {
        gettimeofday(&now,NULL);
        now.tv_sec -= _basetime.tv_sec;
        now.tv_usec -= _basetime.tv_usec;

        if (now.tv_usec < 0)
        {
            now.tv_sec--;
            now.tv_usec += 1000000;
        }

        elapsed =
            _diff_times(
                now,
                pr_times[pid].crt_time
            );

        wait_time =
            _diff_times(
                *elapsed,
                pr_times[pid].run_time
            );


        if (wait_time->tv_sec < 0)
        {
            wait_time->tv_sec = 0;
            wait_time->tv_usec = 0;
        }


        pr_times[pid].wt_time =
            *wait_time;


        pcb->ps_state = PS_DEAD;


        delete_process(pid);


        return 0;
    }

    pcb->ps_state = PS_RDY;
    return 0;
}

void scissos_print_pcb(
    ScisSosProcess *process,
    FILE *fp)
{
    ScisSosPCB *pcb;


    if (process == NULL ||
        fp == NULL)
    {
        return;
    }
    pcb = process->_pcb;
    if (pcb == NULL)
    {
        return;
    }


    fprintf(fp,"\n========== PCB ==========\n");
    fprintf(fp,"Process Name : %s\n",process->_pname);

    fprintf(fp,"PID          : %d\n",pcb->pid);
    fprintf(fp,"UID          : %d\n",pcb->uid);
    fprintf(fp,"Size         : %d\n",pcb->size);
    fprintf(fp,"Priority     : %d\n",pcb->priority_value);
    fprintf(fp,"State        : %d\n",pcb->ps_state);
    fprintf(fp,"Process Type : %d\n",pcb->p_type);
    fprintf(fp,"Memory Type  : %d\n",pcb->m_type);
    fprintf(fp,"PC           : %d\n",pcb->pc);
    fprintf(fp,"Time Slice   : %d\n",pcb->p_timeslice);
    fprintf(fp,"=========================\n");
}

int scissos_proc_save(
    ScisSosProcess *process,
    FILE *fp)
{
    if (process == NULL ||fp == NULL)
    {
        return -1;
    }
    fprintf(fp,"%s %d %d\n",process->_pname,process->_PID,process->_psize);

    return 0;
}


ScisSosProcess *ScisSosPCreator(int number)
{
    ScisSosProcess *creator;
    creator =scissos_proc_create("ProcessCreator",24000,PT_REG,MT_GOOD);

    if (creator == NULL)
    {
        return NULL;
    }
    creator_remaining = number;
    creator_pid = creator->_PID;
    return creator;
}

int *memory_gen_addrefstrings(
    int a,
    int b)
{
    (void)a;
    (void)b;

    return NULL;
}