#include <iostream>
#include <string>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT "23769"  // Use your dynamic port number here

int main() {
    int sockfd, numbytes;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    std::cout << "Client is up and running." << std::endl;
    // int clientNum = 1; // Initialize client number

    while (true) {
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((rv = getaddrinfo("localhost", PORT, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
        }

        // Loop through all the results and connect to the first available
        for (p = servinfo; p != nullptr; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                perror("client: socket");
                continue;
            }

            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                perror("client: connect");
                close(sockfd);
                continue;
            }

            break;
        }

        freeaddrinfo(servinfo); // All done with this structure

        if (p == nullptr) {
            fprintf(stderr, "client: failed to connect\n");
            return 2;
        }

        // Prompt user to input a department name
        std::string departmentName;
        std::cout << "Enter Department Name: ";
        std::cin >> departmentName;

        // Send the department name to the Main server
        if (send(sockfd, departmentName.c_str(), departmentName.size(), 0) == -1) {
            perror("send");
            close(sockfd);
            exit(1);
        }

        std::cout << "Client has sent Department " << departmentName << " to Main Server using TCP." << std::endl;

        char buf[1024];
        memset(buf, 0, sizeof(buf));

        // Receive and display the reply from the Main server
        if ((numbytes = recv(sockfd, buf, sizeof(buf) - 1, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';
        std::cout << "Main Server: " << buf << std::endl;

        close(sockfd);
        std::cout << "-----Start a new query-----" << std::endl;
        // std::cout << "Enter Department Name: ";

        // clientNum++; // Increment client number for the next instance
    }

    return 0;
}
