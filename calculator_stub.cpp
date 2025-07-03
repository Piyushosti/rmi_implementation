#include "calculator_service.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

class CalculatorStub : public CalculatorService {
private:
    int sock;

    double invoke(const std::string& method, double a, double b) {
        std::string request = method + " " + std::to_string(a) + " " + std::to_string(b);
        char buffer[BUFFER_SIZE] = {0};

        send(sock, request.c_str(), request.length(), 0);
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            std::cerr << "Server disconnected" << std::endl;
            return 0.0;
        }

        std::string response(buffer);
        try {
            return std::stod(response);
        } catch (...) {
            std::cerr << "Error: " << response << std::endl;
            return 0.0;
        }
    }

public:
    CalculatorStub() {
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cerr << "Socket creation error" << std::endl;
            return;
        }

        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address" << std::endl;
            return;
        }

        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Connection failed" << std::endl;
            return;
        }
    }

    double add(double a, double b) override {
        return invoke("add", a, b);
    }

    double subtract(double a, double b) override {
        return invoke("subtract", a, b);
    }

    ~CalculatorStub() {
        close(sock);
    }
};
