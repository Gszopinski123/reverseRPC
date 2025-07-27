#ifndef RRPC_SERVER_H_

#define RRPC_SERVER_H_

#include <iostream>
#include <unordered_map>
#include <functional>

namespace rrpc {
    
    class RrpcServer {
        public:
            RrpcServer();
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