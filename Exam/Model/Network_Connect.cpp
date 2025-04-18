#include "Network_Connect.h"

NetworkInterface* Network_Connect::Connect(){
    
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
        while (true) {
            ThisThread::sleep_for(500ms);
        }
    } else {
        printf("IP Acquired: %s\n", local_address.get_ip_address());
    }
    
    return network;
}
