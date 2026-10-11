# SCISSOS – Operating Systems Assignment

## Overview

SCISSOS is a simple operating system simulator implemented in C. It demonstrates process creation, process execution, ready-queue management, CPU scheduling, and process timing measurements.

The simulator supports three scheduling algorithms:

- **First-Come, First-Served (FCFS):** Selects the process at the front of the ready queue.
- **Priority Scheduling:** Selects the ready process with the smallest numerical priority value.
- **Longest Job First (LJF):** Selects the ready process with the largest remaining amount of work.

The scheduling algorithm is selected through a command-line argument. The simulator displays a compact dispatch table showing process execution progress and a final table containing process timing measurements.


## Compilation and Execution

### Step 1: Compile the source files

Compile each source file separately:

```bash
gcc -Wall -Wextra -std=c11 -c scissos_processes.c -o scissos_processes.o
gcc -Wall -Wextra -std=c11 -c scissos_os.c -o scissos_os.o
gcc -Wall -Wextra -std=c11 -c scheduler.c -o scheduler.o
gcc -Wall -Wextra -std=c11 -c main.c -o main.o
```

These commands generate four object files:

```text
scissos_processes.o
scissos_os.o
scheduler.o
main.o
```

### Step 2: Link the object files

Combine the object files to create the executable:

```bash
gcc scissos_processes.o scissos_os.o scheduler.o main.o -o scissos
```

This creates an executable named `scissos`.

### Step 3: Run the simulator

The program requires exactly one command-line argument specifying the scheduling algorithm.

**Run FCFS:**

```bash
./scissos fcfs
```

**Run Priority Scheduling:**

```bash
./scissos priority
```

**Run LJF:**

```bash
./scissos ljf
```

If an invalid argument is supplied, the program displays a usage or error message.

## Simulation Output

The simulator displays the selected scheduling algorithm, a dispatch table, and the final process timing measurements.

### Dispatch table

The dispatch table records each scheduling decision.

| Column | Description |
|---|---|
| `Dispatch` | Sequential number of the scheduling decision. |
| `PID` | Process ID selected for execution. |
| `PC Before` | Program counter before the execution turn. |
| `PC After` | Program counter after the execution turn. |
| `Result` | Indicates whether the process remains ready or has terminated. |

The program counter (PC) represents the position of the next instruction to execute.

A process can be selected multiple times if it remains ready after an execution turn.

### Process timing table

At the end of the simulation, the program displays timing measurements for the created processes.

| Column | Description |
|---|---|
| `PID` | Process identifier. |
| `Creation` | Time elapsed between simulator initialization and process creation. |
| `Response` | Time from process creation until its first execution. |
| `Waiting` | Estimated accumulated waiting time, calculated by the simulator. |
| `Running` | Accumulated wall-clock time measured during process execution. |

The timing values are displayed in seconds.


## Saving Output to a File

To save the output of a particular scheduling run:

```bash
./scissos fcfs > output_fcfs.txt
```

To view the saved output:

```bash
cat output_fcfs.txt
```

To append another run's output without overwriting the file:

```bash
./scissos priority > output_priority.txt
```

The `>` operator overwrites an existing output file, while `>>` appends to it.

To save the LJF output separately:

```bash
./scissos ljf > output_ljf.txt
```
