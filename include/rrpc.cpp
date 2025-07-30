
#include "rrpc.hpp" 


rrpc::RrpcServer::RrpcServer(int port) {
    this->socket_fd = socket(AF_INET, SOCK_STREAM,0);
    if (this->socket_fd == -1) {
        std::cerr << "Error Creating Socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->servAddress.sin_family = AF_INET;
    this->servAddress.sin_addr.s_addr = INADDR_ANY;
    this->servAddress.sin_port = htons(port);
    if (bind(this->socket_fd, (struct sockaddr*)&servAddress, sizeof(servAddress)) == -1) {
        std::cerr << "Error Binding Socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(this->socket_fd, 0) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        exit(EXIT_FAILURE);
    }
} 
rrpc::RrpcServer::~RrpcServer() {
    std::cout << "Closing Socket on port: " << this->port << std::endl;
    close(this->socket_fd);
}

rrpc::RrpcClient::RrpcClient() {
}
rrpc::RrpcClient::~RrpcClient() {
    
}