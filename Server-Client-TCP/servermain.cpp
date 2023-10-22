#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <signal.h>
#include <set>

#define PORT "23769"  // Use your dynamic port number here
#define BACKLOG 3  // Number of concurrent connections to listen to

// Function to get sockaddr, IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

std::map<std::string, int> departmentBackendMap;
struct sockaddr_in client_addr; // Declare client address structure

void readListFile() {
    std::ifstream file("list.txt");
    std::string line;
    int currentID = -1;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line[0] >= '0' && line[0] <= '9') {
            currentID = std::stoi(line);
        } else {
            size_t pos = 0;
            while ((pos = line.find(";")) != std::string::npos) {
                std::string department = line.substr(0, pos);
                departmentBackendMap[department] = currentID;
                line.erase(0, pos + 1);
            }
            // Process the last department name in the line
            if (!line.empty()) {
                departmentBackendMap[line] = currentID;
            }
        }
    }
}

int currentClientID = 1; // Initialize client ID counter

void handleClient(int new_fd) {
    char buf[100];
    int numbytes;
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof client_addr;
    char clientPort[6];

    if (getpeername(new_fd, (struct sockaddr*)&client_addr, &addr_size) == -1) {
        perror("getpeername");
        exit(1);
    }

    inet_ntop(AF_INET, &client_addr.sin_addr, buf, sizeof client_addr);
    sprintf(clientPort, "%d", ntohs(client_addr.sin_port));

    int clientID = currentClientID;

    char departmentName[100];
    memset(departmentName, 0, sizeof(departmentName));

    numbytes = recv(new_fd, departmentName, sizeof(departmentName) - 1, 0);
    if (numbytes == -1) {
        perror("recv");
        exit(1);
    }

    std::cout << "Main server has received the request on Department " << departmentName
              << " from client " << clientID << " using TCP over port " << clientPort << ".\n";

    // Handle department name and backend server ID lookup here
    std::string replyMsg;

    // Simulate the department lookup (replace with your actual lookup logic)
    int backendServerID = departmentBackendMap[departmentName];

    if (backendServerID != 0) {
        // Department found, prepare the reply message
        replyMsg = "Department " + std::string(departmentName) + " is associated with Backend server " + std::to_string(backendServerID) + ".";
        std::cout << "Main Server has sent searching result to client " << clientID
                  << " using TCP over port " << clientPort << ".\n";
    } else {
        // Department not found, prepare an error message
        replyMsg = std::string(departmentName) + " does not show up in the backend server";

        // Define a set to collect backend server IDs
        std::set<int> backendServerIDs;

        // Collect all backend server IDs (assuming they are stored in a set)
        for (const auto& entry : departmentBackendMap) {
            backendServerIDs.insert(entry.second);
        }

        // Append the list of backend server IDs to the error message
        for (int id : backendServerIDs) {
            replyMsg += " " + std::to_string(id) + ",";
        }
        // Remove the trailing comma
        if (!backendServerIDs.empty()) {
            replyMsg.pop_back();
        }
    }

    if (send(new_fd, replyMsg.c_str(), replyMsg.size(), 0) == -1) {
        perror("send");
    }

    close(new_fd); // Close the client socket when the session is done
}

// Signal handler to reap child processes
void sigchld_handler(int s) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    std::cout << "Main server is up and running. \n";
    std::cout << "           \n";
    
    readListFile();
    std::cout << "Main server has read the department list from list.txt.\n";
    std::cout << "           \n";

    int totalBackendServers = 0;
    for (const auto& entry : departmentBackendMap) {
        totalBackendServers = std::max(totalBackendServers, entry.second);
    }
    // Count and print distinct departments for each Backend Server
    int backServ = 0;
    for (int i = 1; i <= totalBackendServers; ++i) {
        int distinctDepartments = 0;
        for (const auto& entry : departmentBackendMap) {
            if (entry.second == i) {
                distinctDepartments++;
            }
        }
        if (distinctDepartments > 0) {
            backServ++;
            std::cout << "Backend Server " << i << " contains " << distinctDepartments << " distinct departments" << std::endl;
        }
    }
    std::cout << "Total number of Backend Servers: " << backServ << std::endl;   
    std::cout << "           \n";

    // Create a socket to listen for incoming connections
    int sockfd, new_fd;  // listen on sockfd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    int yes = 1;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int rv;

    char s[INET6_ADDRSTRLEN];
    // Set up a socket to listen for incoming connections
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // Loop through the results and bind to the first available socket
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // All done with this structure

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // Reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // printf("server: waiting for connections...\n");

    while (1) {  // Main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        // printf("server: got connection from %s\n", s);

        if (!fork()) { // This is the child process
            close(sockfd); // Child doesn't need the listener

            // Call the handleClient function to handle the client request
            handleClient(new_fd);

            close(new_fd);
            exit(0);
        }
        close(new_fd);  // Parent doesn't need this

        currentClientID++; // Increment client ID for the next client
    }

    return 0;
}