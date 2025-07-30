all: clean build


CC = g++ 
SRC_PATH = src/
INCLUDE_PATH = include
BUILD_PATH = build/executables/
build: $(SRC_PATH)rrpc_client.cpp $(SRC_PATH)rrpc_server.cpp
	$(CC) -o $(BUILD_PATH)rrpc_client -I$(INCLUDE_PATH) $(SRC_PATH)rrpc_client.cpp $(INCLUDE_PATH)/rrpc.cpp 
	$(CC) -o $(BUILD_PATH)rrpc_server -I$(INCLUDE_PATH) $(SRC_PATH)rrpc_server.cpp $(INCLUDE_PATH)/rrpc.cpp 

clean: 
	rm -rf $(BUILD_PATH)* 