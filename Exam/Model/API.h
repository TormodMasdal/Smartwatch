#ifndef API_H
#define API_H

#include "iostream"
#include "NetworkInterface.h" 
#include "Network_Connect.h"


class API{
public:

    TLSSocket socket;

    void Open_Socket(const char Web_Name[], const char SSL_CA_PEM[], NetworkInterface* network);

    void Send_Request(const std::string& url);
    
    std::string Read_Request();

    void Close_Socket();
};


#endif

//Function for fetching the external IP
std::string Api_Fetch_External_IP(API& Api, NetworkInterface * network, const char root_ca_cert[]);

//Function for a general API fetch
std::string Api_Fetch(API& Api, std::string hostname, std::string url, const char root_ca_cert[], NetworkInterface* network);