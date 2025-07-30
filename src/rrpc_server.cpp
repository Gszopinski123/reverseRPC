#include "rrpc.hpp"



int main(void) {
    rrpc::RrpcServer myServer = rrpc::RrpcServer(8080);
    if (myServer.connect() < 0) {
        std::cerr << "Connection Failure" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Connection Success" << std::endl;
    return 0;
} 