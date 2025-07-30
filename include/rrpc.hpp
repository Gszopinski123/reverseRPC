#ifndef RRPC_SERVER_H_

#define RRPC_SERVER_H_

#include <iostream>
#include <unordered_map>
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace rrpc {
    
    class RrpcServer {
        private:
            int port;
            int socket_fd;
            sockaddr_in servAddress;
        public:
            RrpcServer(int port);
            ~RrpcServer();
            int send();
            int run();
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