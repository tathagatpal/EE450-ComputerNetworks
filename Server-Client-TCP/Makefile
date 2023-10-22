CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGS =

# List of source files for the main server and client
SERVER_SRCS = servermain.cpp
CLIENT_SRCS = client.cpp

# Names for the output executables
SERVER_EXEC = servermain
CLIENT_EXEC = client

all: $(SERVER_EXEC) $(CLIENT_EXEC)

$(SERVER_EXEC): $(SERVER_SRCS)
	$(CC) $(CFLAGS) $(SERVER_SRCS) -o $(SERVER_EXEC) $(LDFLAGS)

$(CLIENT_EXEC): $(CLIENT_SRCS)
	$(CC) $(CFLAGS) $(CLIENT_SRCS) -o $(CLIENT_EXEC) $(LDFLAGS)

clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC)

.PHONY: all clean

