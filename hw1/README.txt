# Homework 1: A Virtual Machine for PM/0

## Description
A brief overview of your virtual machine implementation, explaining its purpose and functionality.
A struct, called Instruction, is used to hold three variables, from the global array pas, 
called op (operation), L (lexicographical level), and M which indicates several different
operations depending on the operator. During a while loop instructions are loaded into each part of
the struct a switch statement reads from In.op and performs a specific operation based on 
the value held in In.op. After performing the operation, as well as any required updates, 
the print function is called to display the current PC, BP, SP and stack, additionally
the operation that was performed, its lexicographical level, and the value of M.

## Compilation Instructions
Detailed commands to compile your code:
```bash
gcc vm.c -o a.out
```

## Usage
How to run your compiled program, including any necessary arguments:
```bash
# Example usage
./a.out [arguments]
```

## Example

Provide a simple example to illustrate how to use your program:
```bash
# Example command and expected output
./virtual_machine modified_input.txt
Output: Result of processing modified_input.txt
        		 PC  BP  SP  stack 
Initial values:  10  499 500 

  JMP 0 55 	 55  499 500 
  INC 0 5 	 58  499 495 0 0 0 0 0 
Please enter an integer: 3
  SYS 0 2 	 61  499 494 0 0 0 0 0 3 
  CAL 0 16 	 16  493 494 0 0 0 0 0 3 | 
  INC 0 4 	 19  493 490 0 0 0 0 0 3 | 499 499 64 0 
  LIT 0 4 	 22  493 489 0 0 0 0 0 3 | 499 499 64 0 4 
  LIT 0 3 	 25  493 488 0 0 0 0 0 3 | 499 499 64 0 4 3 
  MUL 0 3 	 28  493 489 0 0 0 0 0 3 | 499 499 64 0 12 
  STO 1 4 	 31  493 490 0 0 0 0 12 3 | 499 499 64 0 
  LIT 0 14 	 34  493 489 0 0 0 0 12 3 | 499 499 64 0 14 
  LOD 1 4 	 37  493 488 0 0 0 0 12 3 | 499 499 64 0 14 12 
  LSS 0 7 	 40  493 489 0 0 0 0 12 3 | 499 499 64 0 0 
  JPC 0 49 	 49  493 490 0 0 0 0 12 3 | 499 499 64 0 
  LIT 0 5 	 52  493 489 0 0 0 0 12 3 | 499 499 64 0 5 
  RTN 0 0 	 64  499 494 0 0 0 0 12 3 
Output result is: 3
  SYS 0 1 	 67  499 495 0 0 0 0 12 
  SYS 0 3 	 70  499 495 0 0 0 0 12 

```

## Contact Information
For any questions or issues, please contact:

Name:  Joshua Bandy

Email: jo394902@ucf.edu
