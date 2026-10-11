#ifndef SCISSOS_H
#define SCISSOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

//Constants defining OS parameters

#define MAXPROC     1000
#define MAXUSRS     10
#define DEFPRIO     20
#define EMPTY       -100
#define MAXPGES     10
#define DEFTS       6000
#define REG_THR     0.02
#define CMP_THR     0.001
#define IOE_THR     0.2
#define MAXPROCNAME 80

/* scheduling algorithm */
#define SCHED_FCFS      1
#define SCHED_PRIORITY  2
#define SCHED_LJF       3


//Constants for Process States 
#define PS_NEW   0
#define PS_RDY   1
#define PS_RUN   2
#define PS_BLK   3
#define PS_SRDY  4
#define PS_SBLK  5
#define PS_DEAD  6


//Constants for Process Types
#define PT_REG  0
#define PT_CMP  1
#define PT_IOE  2


//Memory Types

#define MT_GOOD  0
#define MT_BAD   1
#define MT_UGLY  2


//Instruction Types

#define INS_LNG  10
#define INS_SHR  20

#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define MIN(A, B) (((A) < (B)) ? (A) : (B))


typedef int ScisSosPGTable[2];

/** Instruction in a process; process is a sequence of instructions **/
typedef struct
{
    int _inum;
    int _syscall;
    int _addref;

} ScisSosInst;

/** Process Control Block structure **/
typedef struct
{
    int pid;
    int uid;
    int size;
    int priority_value;
    int ps_state;
    int p_type;
    int m_type;
    int pc;
    ScisSosInst **p_code;
    ScisSosPGTable pg_table[MAXPGES];
    int p_timeslice;
} ScisSosPCB;

/** Process Structure **/
typedef struct
{
    char _pname[MAXPROCNAME];
    int _PID;
    int _psize;
    ScisSosPCB *_pcb;
    ScisSosInst **_CODE;
} ScisSosProcess;

/** Data structures used by the OS to do its management actions **/
/** Defined in the scissos_os.c file                      **/
extern ScisSosPCB *_proctable[MAXPROC];
extern int _readyQ[MAXPROC];
extern int _blockQ[MAXPROC];


/** Time maintenance structure **/
typedef struct
{
    struct timeval crt_time;
    struct timeval rsp_time;
    struct timeval wt_time;
    struct timeval run_time;

} TIMING;

extern struct timeval _basetime;
extern TIMING pr_times[MAXPROC];

struct timeval *_diff_times(struct timeval, struct timeval);
struct timeval *_add_times(struct timeval, struct timeval);
struct timeval _max_times(struct timeval, struct timeval);

//Process functions
ScisSosProcess *scissos_proc_create(char *,int,int,int);
ScisSosProcess *ScisSosPCreator(int);
int scissos_proc_save(ScisSosProcess *,FILE *);
void scissos_print_pcb(ScisSosProcess *,FILE *);
int scissos_proc_run(int);

//OS functions
void scissos_initialise(void);
void scissos_call_scheduler(void);
void scissos_print_timings(void);

//Scheduling algorithm
typedef int (*SchedulerFunc)(int readyQ[], int qsize);

extern SchedulerFunc scheduler_fn;
extern int scheduler_mode;

void scissos_set_scheduler(SchedulerFunc fn, int mode);

int sched_fcfs(int readyQ[], int qsize);
int sched_priority(int readyQ[], int qsize);
int sched_ljf(int readyQ[], int qsize);

//Memory management
int *memory_gen_addrefstrings(int, int);

#endif