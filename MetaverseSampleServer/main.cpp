#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_BUFFER_SIZE];
    int addr_len;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock." << std::endl;
        return -1;
    }

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
        return -1;
    }

    // Listen to incoming connections
    listen(server_socket, 3);

    std::cout << "Server is listening on port 8080..." << std::endl;

    // Accept incoming connection
    addr_len = sizeof(struct sockaddr_in);
    while (1) {
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len)) == INVALID_SOCKET) {
            std::cerr << "Accept failed." << std::endl;
            return -1;
        }

        std::cout << "Connection accepted." << std::endl;

        memset(buffer, 0, MAX_BUFFER_SIZE);
        std::string data = "";

        // Echo server
        while (recv(client_socket, buffer, MAX_BUFFER_SIZE, 0) != 0) {

            data = buffer;
            memset(buffer, 0, MAX_BUFFER_SIZE);

            std::cout << "Client: " << data << std::endl;

            send(client_socket, data.c_str(), data.size(), 0);
        }

        closesocket(client_socket);

    }
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
