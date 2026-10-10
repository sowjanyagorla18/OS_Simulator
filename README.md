# OS_Simulator
# SCISSOS – Operating Systems Assignment

## Overview

This project implements a simple operating system simulation using C. It consists of multiple source files that are compiled separately and linked together to generate the executable program `scissos`.

## Prerequisites

Make sure the following are installed on your system:

- **GCC compiler** – to compile the C source files.
- **Terminal** – to execute the compilation and run commands.
- **Linux or WSL** – recommended for running the program using the commands below.

To check whether GCC is installed, run:

```bash
gcc --version
```

## Project Files

Ensure that the following files are present in the project directory:

```text
project/
├── scissos_processes.c
├── scissos_os.c
├── scheduler.c
├── main.c
└── README.md
```

## How to Compile and Run

### Step 1: Open the Terminal

Navigate to the directory containing the source files:

```bash
cd path/to/project
```

Replace `path/to/project` with the actual path to your project directory.

### Step 2: Compile the Source Files

Compile each C source file separately using GCC:

```bash
gcc -Wall -Wextra -std=c11 -c scissos_processes.c -o scissos_processes.o
gcc -Wall -Wextra -std=c11 -c scissos_os.c -o scissos_os.o
gcc -Wall -Wextra -std=c11 -c scheduler.c -o scheduler.o
gcc -Wall -Wextra -std=c11 -c main.c -o main.o
```

**Explanation:**

- `gcc` invokes the GNU C compiler.
- `-Wall` enables common compiler warnings.
- `-Wextra` enables additional warnings.
- `-std=c11` specifies the C11 standard.
- `-c` compiles the source file into an object file without linking.
- `-o` specifies the name of the output file.

This step generates the following object files:

```text
scissos_processes.o
scissos_os.o
scheduler.o
main.o
```

### Step 3: Link the Object Files

Combine the object files to create the executable:

```bash
gcc scissos_processes.o scissos_os.o scheduler.o main.o -o scissos
```

This command links the compiled object files and produces an executable named `scissos`.

### Step 4: Run the Program

Execute the program using:

```bash
./scissos
```

The program runs in the terminal, and its output is displayed on the screen.

### Step 5: Save the Output to a File

To redirect the program's output to a text file, run:

```bash
./scissos > output.txt
```

This command saves the program's standard output in `output.txt` instead of displaying it in the terminal.

**Note:** If `output.txt` already exists, its contents will be overwritten.

To display the output saved in the file, use:

```bash
cat output.txt
```

To append new output without overwriting the existing contents, use:

```bash
./scissos >> output.txt
```

## Rebuilding the Project

If you modify any source file, recompile the corresponding object file and repeat the linking step.

For example, if `scheduler.c` is modified:

```bash
gcc -Wall -Wextra -std=c11 -c scheduler.c -o scheduler.o
gcc scissos_processes.o scissos_os.o scheduler.o main.o -o scissos
```

Then run the executable again:

```bash
./scissos > output.txt
```

## Cleaning Generated Files

To remove the generated object files, executable, and output file, run:

```bash
rm -f *.o scissos output.txt
```

This command removes generated files from the current directory that match the specified names or patterns. It does not delete the original C source files or this README.

## Troubleshooting

- **`gcc: command not found`** – Install GCC or configure your compiler path.
- **`No such file or directory`** – Verify that you are in the correct project directory and that all source files exist.
- **`undefined reference` errors** – Check that all required object files are included in the linking command and that the required function definitions are available.
- **Compiler warnings** – Review warnings carefully and fix any issues where necessary.
- **Permission denied when running `./scissos`** – Check the file permissions and confirm that the executable was created successfully.

## Author

gorla padma venkata sowjanya
d. vishnu suprita
