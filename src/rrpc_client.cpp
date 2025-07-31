#include "rrpc.hpp"

void hello_world(rrpc::RrpcArgument arg) {

    std::cout << "Function: hello_world, has been called" << std::endl;

}


int main(void) {
    rrpc::RrpcClient myClient = rrpc::RrpcClient("127.0.0.1", 8080);
    myClient.bind(&hello_world, "Hello_World");
    myClient.run();
    std::cout << "Connection Successful" << std::endl;
    return 0;
} 