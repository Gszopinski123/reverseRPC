#ifndef RRPC_SERVER_H_

#define RRPC_SERVER_H_
#define NO_FLAGS 0
#define BACKGROUND_QUEUE 0
#include <iostream>
#include <unordered_map>
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>

namespace rrpc {
    typedef struct msgHdr {
        int type;
        int size;
    } MsgHdr;
    class RrpcArgument {
        //change this to your specfic needs
    };
    class RrpcServer {
        private:
            int port;
            int socket_fd;
            int accept_fd;
            sockaddr_in servAddress;
            sockaddr_in clientAddress;
        public:
            RrpcServer(int port);
            ~RrpcServer();
            int send(const char* funct);
            int send(const char* funct, int flags);
            int connect();
    };
    class RrpcClient {
        public:
            RrpcClient(const char *address,int port);
            ~RrpcClient();
            int bind(void (*function)(rrpc::RrpcArgument), const char* functionName);
            int run();
        private:
            std::unordered_map<std::string, std::function<void(rrpc::RrpcArgument)>> function_entries;
            int socket_fd;
            int port;
            struct sockaddr_in servAddress;
            struct hostent *servername;
    };

    
    
}



#endif