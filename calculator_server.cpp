#include "calculator_service.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#define PORT 8080
#define BUFFER_SIZE 1024

// Concrete Calculator Service implementation
class CalculatorServiceImpl : public CalculatorService {
public:
    double add(double a, double b) override { return a + b; }
    double subtract(double a, double b) override { return a - b; }
   double multiply(double a, double b) override { return a * b; }
    double divide(double a, double b) override {
        if (b == 0) return 0; 
        return a / b;
    }
};

// Handle client requests
void handleClient(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    CalculatorServiceImpl calc;

    while (true) {
        int valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        std::string request(buffer);
        std::stringstream ss(request);
        std::string method;
        double a, b;

        ss >> method >> a >> b;

         std::string response;
        if (method == "add") {
            response = std::to_string(calc.add(a, b));
        } else if (method == "subtract") {
            response = std::to_string(calc.subtract(a, b));
        } else if (method == "multiply") {
            response = std::to_string(calc.multiply(a, b));
        } else if (method == "divide") {
            if (b == 0) {
                response = "Error: Division by zero";
            } else {
                response = std::to_string(calc.divide(a, b));
            }
        } else {
            response = "Invalid method";
        }
 send(client_socket, response.c_str(), response.length(), 0);
        memset(buffer, 0, BUFFER_SIZE);
    }
    close(client_socket);
}

// Start the server
void startServer() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(client_addr);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "Setsockopt failed" << std::endl;
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        int new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }
        std::cout << "New client connected" << std::endl;
        std::thread client_thread(handleClient, new_socket);
        client_thread.detach();
    }
    close(server_fd);
}
