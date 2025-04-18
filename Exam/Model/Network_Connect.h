#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "mbed.h"
#include "NetworkInterface.h"


// CONNECT TO NETWORK AND DISPLAY LOCAL IP ADDRESS
class Network_Connect{

    
public: 
    NetworkInterface* Connect();
};


#endif