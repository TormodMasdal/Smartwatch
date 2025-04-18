#ifndef API_H
#define API_H

#include "iostream"
#include "NetworkInterface.h" 
#include "Network_Connect.h"

// CONNECT AND FETCH API
class API{
public:
    TLSSocket socket;
    void Open_Socket(const char Web_Name[], const char SSL_CA_PEM[], NetworkInterface* network);
    void Send_Request(const char http_request[]);
    void read_Request();    
};

#endif