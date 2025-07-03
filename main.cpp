#include "calculator_service.h"
#include "calculator_server.cpp"
#include "calculator_stub.cpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [server|client]" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "server") {
        startServer();
    } else if (mode == "client") {
        CalculatorStub calc;
        std::cout << "Client: 5 + 3 = " << calc.add(5, 3) << std::endl;
        std::cout << "Client: 5 - 3 = " << calc.subtract(5, 3) << std::endl;
    } else {
        std::cerr << "Invalid mode. Use 'server' or 'client'" << std::endl;
    }

    return 0;
}
