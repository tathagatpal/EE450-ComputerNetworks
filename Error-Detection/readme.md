README for Error Detection Assignment

1) Full Name: Tathagat Pal
2) Student ID: 5362141769
3) Platform: Ubuntu 20.04.6 LTS

(4) Assignment Description:
In this assignment, we were tasked with implementing error detection mechanisms, such as CRC-8, CRC-16, 2D Parity, and Checksum, for a given set of data. The goal of the assignment is to demonstrate your understanding of these error detection techniques and their practical application.

Implemented the error detection algorithms, read data from a file, introduced simulated errors based on an error pattern, and checked whether the error detection mechanisms correctly identify these errors.

(5) Code Files:
    ee450_PA1_tpal.cpp: This is the main C++ source code file that contains the implementation of the error detection mechanisms and the main program logic. It includes functions to calculate CRC-8, CRC-16, checksum, and perform 2D Parity error detection. The code reads data from an input file, introduces errors, checks for errors, and displays the results.
    
    
6) Idiosyncrasies:
   - The program assumes that the input data file is named "data.txt" and is located in the same directory as the executable.
   - It expects each line of the input file to contain binary data followed by error bits separated by a space.
   - Error detection is performed for each line of data independently.
   
7) Project Failures:
   - The project may fail to work correctly if the input data file ("data.txt") is missing or not formatted as expected (binary data followed by error bits separated by a space).
   - The program assumes that the input data size is consistent with the error detection method being used (e.g., 8 bits for CRC-8, 16 bits for CRC-16).
   - Errors introduced in the input data may not be correctly detected if the error pattern is not properly defined.
   
8) Reused Code:
   - The code provided in this program is developed from scratch and does not reuse code from external sources.

To compile and run the program, use the provided Makefile with the following commands:
- `make all` to compile the program.
- `make run` to run the program and view the results on the terminal.
- `make clean` to clean up build artifacts and output files.

Please make sure you have the necessary input data file ("data.txt") formatted correctly with binary data and error bits for testing the error detection mechanisms.

