
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
    if (this->accept_fd > 0)
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

rrpc::RrpcReturn rrpc::RrpcServer::send(const char* funct, RrpcArgument argument) {
    int functLen = strlen(funct);
    int type = RPC_CALL;
    MsgHdr *msgHdr;
    char *buffer = (char*)malloc((sizeof(char)*functLen)+sizeof(MsgHdr)+sizeof(RrpcArgument));
    size_t sizeOfBuffer = sizeof(char)*functLen + sizeof(MsgHdr) + sizeof(RrpcArgument);
    msgHdr = (MsgHdr*)buffer;
    msgHdr->size = functLen;
    msgHdr->type = type;
    RrpcReturn premature_failure;
    premature_failure.isSuccess = false;
    char *recvBuffer = (char*)malloc(sizeof(RrpcReturn)+sizeof(msgHdr));
    size_t sizeOfrecvBuffer = sizeof(RrpcReturn)+sizeof(msgHdr);
    memcpy(msgHdr+1,funct,functLen);
    memcpy(((char*)(msgHdr+1))+functLen,&argument, sizeof(argument));
    if (this->accept_fd < 0) {
        free(buffer);
        free(recvBuffer);
        return premature_failure;
    }
    int bytes_sent = ::send(this->accept_fd,buffer,sizeOfBuffer,NO_FLAGS);
    if (bytes_sent <= 0) {
        free(buffer);
        free(recvBuffer);
        return premature_failure;
    }

    int bytes_recv = recv(this->accept_fd,recvBuffer,sizeOfrecvBuffer, NO_FLAGS);
    if (bytes_recv <= 0) {
        free(buffer);
        free(recvBuffer);
        return premature_failure;
    }
    MsgHdr* hdrBack = (MsgHdr*)recvBuffer;
    if (hdrBack->type != SEND_BACK) {
        free(buffer);
        free(recvBuffer);
        return premature_failure;
    }
    rrpc::RrpcReturn *returnBack = (rrpc::RrpcReturn*)(hdrBack+1);
    free(buffer);
    free(recvBuffer);
    return *returnBack;

}
rrpc::RrpcReturn rrpc::RrpcServer::send(const char* funct, RrpcArgument argument, int flags) {
    //connect to database and see if user is authorized
    return this->send(funct,argument);
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
    if (this->servername != NULL)
        free(this->servername);
    close(this->socket_fd);
}

int rrpc::RrpcClient::run() {
    char * buffer = (char*)malloc(sizeof(MsgHdr)+sizeof(RrpcArgument)+sizeof(char)*512);
    char * bufferBack = (char*)malloc(sizeof(msgHdr)+sizeof(rrpc::RrpcReturn));\
    size_t bufferBackLen = sizeof(msgHdr)+sizeof(rrpc::RrpcReturn);
    size_t bufferlen = sizeof(char)*512+sizeof(MsgHdr)+sizeof(RrpcArgument);
    int bytesRecv = recv(this->socket_fd,buffer,bufferlen, NO_FLAGS);
    while (bytesRecv > 0) {
        MsgHdr* msghdr = (MsgHdr*)buffer;
        std::string functionName((char*)(msghdr+1),msghdr->size);
        RrpcArgument *argument = (RrpcArgument*)(((char*)(msghdr+1))+msghdr->size);
        auto toBeCalled = function_entries.find(functionName);
        rrpc::RrpcReturn *sendback;
        MsgHdr* hdrSendBack;
        hdrSendBack = (MsgHdr*)bufferBack;
        hdrSendBack->size = 0;
        hdrSendBack->type = SEND_BACK;
        sendback = (rrpc::RrpcReturn*)(hdrSendBack+1);
        if (toBeCalled != function_entries.end()) {
            toBeCalled->second(*argument);
            sendback->isSuccess = true;
        } else {
            sendback->isSuccess = false;
        }
        send(this->socket_fd,bufferBack,bufferBackLen,NO_FLAGS);
        memset(buffer, 0, bufferlen);
        bytesRecv = recv(this->socket_fd, buffer, bufferlen,NO_FLAGS);
    }
    free(buffer);
    free(bufferBack);
    return 0;
}


int rrpc::RrpcClient::bind(void (*function)(rrpc::RrpcArgument), const char* functionName) {
    function_entries[std::string(functionName)] = function;
    return 0;
}