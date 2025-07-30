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

namespace rrpc {
    typedef struct msgHdr {
        int type;
        int size;
    } MsgHdr;
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
            int send(char* funct);
            int send(char* funct, int flags);
            int connect();
    };
    class RrpcClient {
        public:
            RrpcClient();
            ~RrpcClient();
            int bind();
        private:
            std::unordered_map<std::string, std::function<void(rrpc::RrpcArgument)>> function_entries;
    };

    
    class RrpcArgument {
        //change this to your specfic needs
    };
}



#endif