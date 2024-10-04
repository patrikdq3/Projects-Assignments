==============================================================================
README for Virtual P-Machine Implementation (vm.c)
==============================================================================

OVERVIEW:
This project implements a Virtual P-Machine, also known as a PL/0 machine, in ANSI standard C.
The program simulates a P-machine, a stack machine which conceptually has one memory area called the process address space (PAS). 
The process address space is divide into two contiguous segments: the "text”, 
which contains the instructions for the VM to execute and the "stack,” 
which is organized as a data-stack to be used by the PM/0 CPU.
The implementation includes basic functionalities such as arithmetic operations, control flow, and stack management instructions.

AUTHORS:
- Alexander Lokhanov
- Patrik De Quevedo

COURSE:
COP3402 - Systems Security, Spring 2024
Instructor: Montagne
Due Date: February 2, 2024

COMPILATION:
Use the following command in a terminal in the folder
    gcc -o vm vm.c -Wall

EXECUTION:
To run the virtual P-Machine, use the following syntax:
    ./vm input.txt

Where 'input.txt' is the input file containing the PL/0 instructions that will be executed by the virtual machine.

FUNCTIONALITY:
The program simulates the execution of a PL/0 program by reading instructions from an input file and performing operations based on those instructions. The main components of the virtual machine include:

- A Program Address Space (PAS) for storing instructions.
- A stack for managing execution contexts, including local variables and return addresses.
- A set of registers (PC, BP, SP) for program counter, base pointer, and stack pointer, respectively.
- Instruction execution based on opcodes, including arithmetic operations, stack operations, and control flow.

The virtual machine executes in a fetch-decode-execute cycle, processing each instruction from the input file sequentially until the end of the program or an explicit halt instruction is encountered.

NOTES:
- The program does not include error handling for invalid instructions or runtime errors. It assumes that the input program is correctly written.

==============================================================================
