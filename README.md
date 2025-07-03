# rmi_implementation
This project implements a simple Remote Method Invocation (RMI)-like system in C++ using TCP sockets. It provides a calculator service where a client can remotely invoke `add` and `subtract` methods on a server.

### Compile and Run 
1. compile:
   
   `g++ -std=c++11 rmi_calculator.cpp -o rmi_calculator -pthread`

2. Run the server (in one terminal):

   `./rmi_calculator server`

3. Run the client (in another terminal):

   `./rmi_calculator client`
