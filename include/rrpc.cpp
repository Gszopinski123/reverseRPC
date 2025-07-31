
#include "rrpc.hpp" 


rrpc::RrpcServer::RrpcServer(int port) {
    this->port = port;
    this->socket_fd = socket(AF_INET, SOCK_STREAM, NO_FLAGS);
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
    if (listen(this->socket_fd, BACKGROUND_QUEUE) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->accept_fd = -1;
} 
rrpc::RrpcServer::~RrpcServer() {
    std::cout << "Closing Socket on port: " << this->port << std::endl;
    close(this->socket_fd);
    close(this->accept_fd);
}

int rrpc::RrpcServer::connect() {
    socklen_t clilen = sizeof(clientAddress);
    this->accept_fd = accept(this->socket_fd, (struct sockaddr *) &clientAddress, &clilen);
    if (this->accept_fd < 0) {
        return(-1);
    }
    return 0;
}

int rrpc::RrpcServer::send(const char* funct) {
    int functLen = strlen(funct);
    int type = 1;
    MsgHdr *msgHdr;
    char *buffer = (char*)malloc((sizeof(char)*functLen)+(sizeof(int)*2));
    size_t sizeOfBuffer = sizeof(char)*functLen + sizeof(int)*2;
    msgHdr = (MsgHdr*)buffer;
    msgHdr->size = functLen;
    msgHdr->type = type;
    char *recvBuffer = (char*)malloc(sizeof(char)*512);
    size_t sizeOfrecvBuffer = 512;
    memcpy(msgHdr+1,funct,functLen);
    if (this->accept_fd < 0) {
        return -1;
    }
    int bytes_sent = ::send(this->accept_fd,buffer,sizeOfBuffer,NO_FLAGS);
    if (bytes_sent <= 0)
        return -1;

    int bytes_recv = recv(this->accept_fd,recvBuffer,sizeOfrecvBuffer, NO_FLAGS);
    if (bytes_recv <= 0) {
        return -1;
    }
    return 0;

}

rrpc::RrpcClient::RrpcClient(const char * address,int port) {
    this->port = port;
    this->socket_fd = socket(AF_INET, SOCK_STREAM, NO_FLAGS);
    if (this->socket_fd < 0) {
        std::cerr << "Socket Creation Failure" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->servAddress.sin_family = AF_INET;
    this->servAddress.sin_port = htons(port);
    this->servername = gethostbyname(address);
    if (this->servername == NULL) {
        std::cerr << "Error: no such host" << std::endl;
        exit(EXIT_FAILURE);
    }
    memcpy(&this->servAddress.sin_addr.s_addr, servername->h_addr, servername->h_length);
    if (connect(this->socket_fd, (struct sockaddr *)&this->servAddress, sizeof(servAddress)) < 0) {
        std::cerr << "Failed Connection" << std::endl;
        exit(EXIT_FAILURE);
    }
}
rrpc::RrpcClient::~RrpcClient() {
    std::cout << "Closing Connection on port: " << this->port << std::endl;
    close(this->socket_fd);
}

int rrpc::RrpcClient::run() {
    char * buffer = (char*)malloc(sizeof(char)*512);
    size_t bufferlen = sizeof(char)*512;
    int bytesRecv = recv(this->socket_fd,buffer,bufferlen, NO_FLAGS);
    std::cout << "Hello?" << std::endl;
    while (bytesRecv > 0) {
        MsgHdr* msghdr = (MsgHdr*)buffer;
        std::cout << "Size of function is: " << msghdr->size << std::endl;
        std::cout << "Type is: " << msghdr->type << std::endl;
        std::string functionName((char*)(msghdr+1),msghdr->size);
        std::cout << "Function Name is: " << functionName << std::endl;
        auto toBeCalled = function_entries.find(functionName);
        if (toBeCalled != function_entries.end()) {
            toBeCalled->second(rrpc::RrpcArgument{});
        } else {
            std::cout << "Function Not Found!" << std::endl;
        }
        bytesRecv = 0; // NEED TO FIX THIS PLACEHOLDER
    }
    return 0;
}


int rrpc::RrpcClient::bind(void (*function)(rrpc::RrpcArgument), const char* functionName) {
    function_entries[std::string(functionName)] = function;
    return 0;
}