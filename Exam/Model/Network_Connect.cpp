#include "Network_Connect.h"
#include "TCPSocket.h"
#include "iostream"
#include <string>
#include <cstring>

NetworkInterface* Network_Connect::Connect(){
    
    //Connects to the internett, added print statements for debugging
    // GET DEFAULT NETWORK INTERFACE & CONNECT
    NetworkInterface *network = NetworkInterface::get_default_instance();
    if(!network){
        printf("Failed to get default network interface\n");
        while (true) {
            ThisThread::sleep_for(500ms);
        }
    } else {
        printf("Default network interface acquired\n");
    }

    printf("Connecting to the network...\n");
    nsapi_size_or_error_t result = network->connect();
    if(result != 0){
        printf("Failed to connect to network: %d\n", result);
        while (true) {
            ThisThread::sleep_for(500ms);
        }
    } else {
        printf("Connected to the network\n");
    }

    // DISPLAY LOCAL IP-ADDRESS
    SocketAddress local_address;
    result = network->get_ip_address(&local_address);
    if(result != 0){
        printf("Failed to acquire IP Address: %d\n", result);
        //Hangs forever, add way for it to retry?
        while (true) {
            ThisThread::sleep_for(500ms);
        }
    } else {
        printf("IP Acquired: %s\n", local_address.get_ip_address());
    }
    
    return network;
}


//Works, get external ip from ipify
std::string Network_Connect::FetchPublicIP() 
{
     TCPSocket socket;
    NetworkInterface* network = NetworkInterface::get_default_instance();
    if (!network) {
        printf("Network not initialized!\n");
        return "";
    }

    // Resolve the API server's address (DNS lookup)
    SocketAddress server;
    if (network->gethostbyname("api.ipify.org", &server) != NSAPI_ERROR_OK) {
        printf("DNS resolution failed\n");
        return "";
    }
    server.set_port(80); // HTTP port

    // Open and connect the socket
    if (socket.open(network) != NSAPI_ERROR_OK || 
        socket.connect(server) != NSAPI_ERROR_OK) {
        printf("Socket error\n");
        return "";
    }

    // Send HTTP GET request
    const char* request = "GET / HTTP/1.1\r\nHost: api.ipify.org\r\nConnection: close\r\n\r\n";
    if (socket.send(request, strlen(request)) < 0) {
        printf("Failed to send request\n");
        socket.close();
        return "";
    }

    // Read the full response (including headers if any)
    std::string response;
    char buffer[128] = {0};
    while (true) {
        int received = socket.recv(buffer, sizeof(buffer) - 1);
        if (received <= 0) break; // Connection closed or error
        buffer[received] = '\0';
        response += buffer;
    }
    socket.close();

    // The IP is the last part of the response (trim whitespace and headers)
    size_t ip_start = response.find_last_not_of(" \t\n\r");
    if (ip_start != std::string::npos) {
        response.resize(ip_start + 1);
    }

    // If there are HTTP headers, skip them (look for \r\n\r\n)
    size_t body_pos = response.find("\r\n\r\n");
    if (body_pos != std::string::npos) {
        return response.substr(body_pos + 4); // Return only the body (IP)
    }

    return response; // If no headers, return as-is
}