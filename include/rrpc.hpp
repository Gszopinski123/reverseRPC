#ifndef RRPC_SERVER_H_

#define RRPC_SERVER_H_
#define NO_FLAGS 0
#define BACKGROUND_QUEUE 0
#define SEND_BACK 0
#define RPC_CALL 1
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
enum privilege {
    OPEN,
    RESTRICTED,
    GUARDED
};

namespace rrpc {
    typedef struct msgHdr {
        int type; // determines if this is an rpc call or send back // 1 -> rpc call 0 -> send back
        int size; // Dependent on if this is a rpc call or send back 
    } MsgHdr;
    class RrpcArgument {
        enum privilege level;
        //change this to your specfic needs
    };
    class RrpcReturn {
        public:
            bool isSuccess;
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
            RrpcReturn send(const char* funct, RrpcArgument argument);
            RrpcReturn send(const char* funct, RrpcArgument argument, int flags);
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