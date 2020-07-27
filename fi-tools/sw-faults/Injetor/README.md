ucXception v1.1 Beta
========


* [Requirements](#requirements)
* [Usage](#usage)
    * [Synopsis](#synopsis)
    * [File](#file)
    * [Options](#options)
* [Limitations](#limitations)


## Requirements

Due to the fault injector was deployed using Java and taking advantage of some features of CDT, a plugin for Eclipse. In order to use the ucXception, we must ensure some requirements. The main requirements are the installation of the following software:

- **Diff/Patch -** Tools used for create patches and manipulation thereof, based on the comparison of files and thus creating patches with differences (rows inserted, changed or removed).
- **Gcc -** GCC with GCC in backend. Compiler for C language used to remove comments from the source code.
- **Java 8 -** Language used for the development of the ucXception, together with the plugin CDT.

The use of injector in Linux is recommended instead of Windows, since it is already included in the system path some tools necessary for their correct usage, such as diff, patch and gcc.

## Usage

In order to be able to use ucXception, it is noteworthy some particularities related to its usage. For example, the structure of command to run it:

#### Synopsis

    java -jar FaultInjetor.jar FILE [OPTION...]

#### File

    .c .cpp

#### Options

| Flag               | Description             |
| ------------------ | ----------------------- |
| -sp                | Silent patch option     |
| -s                 | Silent option           |
| -d                 | Debug option            |
| -k                 | Keep Files              |
| -m                 | Solve Macros            |
| -o                 | Operator selection      |
| -h                 | Help                    |
| -f                 | Number of faults        |
| -c                 | C Parser                |
| -cpp               | C++ Parser              |
| --no-preprocessing | Do not preprocess       |
| --exit-early       | Exit early              |
| --ignore-extension | Ignore extension        |

**Silent patch option      -** Not print information related to the created patch, number, modifications, etc.

**Silent option            -** Not print the readable code by CDT. This is related to the limitations of fault injector, presented in LINK.

**Debug option             -** Print code before the application of each operator, as well as the code to change or remove, even as the final code, after the modifications performed by the operator.

**Keep Files               -** Keeps also the C files with each patch already applied (instead of only keeping the .patch files

**Solve Macros             -** Enables algorithm that attempts to solve macro problems

**Operator selection       -** Apply the ucXception only to an operator. OPERATOR is case-insensitive.

**Help                     -** Show the help menu

**Number of faults         -** Sets the number of faults will be injected, instead of injecting all faults.

**C Parser                 -** Uses the GNU C Parser as provided by Eclipse CDT (default).

**C++ Parser               -** Uses the GNU C++ Parser as provided by Eclipse CDT.

**Do not preprocess        -** Do not preprocess (gcc -E) the file before performing the injections. Used when the injector is called as part of a bigger system.

**Exit early               -** Just write the file after being parsed/formatted by Eclipse CDT and do not do any fault injection.

**Ignore extension         -** Do not enforce that the input file must end in *.c *.cpp *.h ...

Below are presented some examples of execution of fault injector, for different situations.
Execution of ucXception only with the application of operator MIFS, in file *file.c*:

    $ java -jar FaultInjetor.jar file.c -o=MIFS

This execution differs from the previous one by the activation of DEBUG option. With DEBUG option activated, showing information related to the steps that are doing by the fault injector:

    $ java -jar FaultInjetor.jar file.c -d -o=MLAC

For a correct usage of this software, it is necessary having some knowledge regarding the execution of commands through the console.

## Limitations

Macros
