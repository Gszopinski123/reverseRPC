#include "rrpc.hpp"


int main(void) {
    rrpc::RrpcClient myClient = rrpc::RrpcClient("127.0.0.1", 8080);
    std::cout << "Connection Successful" << std::endl;
    return 0;
} 