# Implementation of client-server socket programming using TCP to deploy a Student Performance Analysis System 

## Author
- Full Name: Tathagat Pal
- Student ID: 5362141769

## Platform
- Ubuntu 20.04 LTS

## Project Overview
This project implements a Student Performance Analysis System for a university. It consists of a main server and multiple clients. Clients can send department names to the main server, which then finds the associated backend server ID and sends the result back to the client.

## Code Files
1. `main_server.cpp`: This file contains the implementation of the main server. It handles the following functionalities:
   - Listens for incoming client connections.
   - Reads the department list from `list.txt` and stores it in memory.
   - Handles client queries, decodes department names, and searches for the associated backend server ID.
   - Uses fork() to handle multiple clients concurrently.
   - Sends reply messages to clients with the result or error.

2. `client.cpp`: This file contains the client implementation. It connects to the main server and performs the following tasks:
   - Prompts the user to enter a department name.
   - Sends the department name to the main server using TCP.
   - Displays the reply from the main server.
   - Repeats the query process until manually terminated.


## Message Format
- Bootup Message (Main Server): Main server is up and running.
- Bootup Message (Client): Client is up and running. Enter Department Name:

- Query Message (Client): Client has sent Department <Department Name> to Main Server using TCP.
- Reply Message (Main Server): Department <Department Name> is associated with Backend server <Backend Server ID>.
- Error Message (Main Server): <Department Name> does not show up in backend server < Backend Server ID 1, Backend Server ID 2, …>

- New Query Message (Client): 
-----Start a new query----- 
Enter Department Name:

## Idiosyncrasy
- Upon executing the make all command, there might be the following warning:

client.cpp:17:10: warning: unused variable ‘s’ [-Wunused-variable]
   17 |     char s[INET6_ADDRSTRLEN];
      |          ^

However, we can ignore this and further execute Main server and clients.

- The project is designed to run on Ubuntu 20.04 LTS. It may not work as expected on other versions older than Ubuntu 20.
-  Department names are composed of letters and can vary from 1 to 20 characters in length. They may include both capital and lowercase letters but should not contain any white spaces.
-  Backend server IDs are represented as non-negative integer numbers and serve as unique identifiers.
-  The system accommodates up to 10 distinct Backend server IDs.
-  The list.txt file should not have additional empty lines at the beginning or end, ensuring data integrity.
-  There is no overlap of department names among different Backend servers, although the same department can be assigned to a single Backend server multiple times.
-  The list.txt file is expected to contain data and should not be empty.
-  Each Backend server can store at least one department name, with a maximum capacity of 100 department names.


## Reused Code
Beej's guide (https://beej.us/guide/bgnet/html/) was used as a reference to implement server and client codes. 
