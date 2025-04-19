#include "mbed.h"
#include "NetworkInterface.h"
#include "Network_Connect.h"
#include <iostream>
#include "API.h"
#include "cerificate.h"



int main() 
{

    Network_Connect network_manager;

    API api;

    printf("\nStarting Network Test...\n");

    NetworkInterface* network = network_manager.Connect();

    while(true) 
    {
        printf("Network is active!\n");
        
        //start api 
        //pass this  addr.get_ip_address() for ip geolocation
        std::string Current_Public_IP = network_manager.FetchPublicIP();
        if (!Current_Public_IP.empty()){
            std::cout<<"Current External Ip: " << Current_Public_IP<<std::endl;
            std::string hostname = "api.ipgeolocation.io";
            std::string Ip_Geo_Url= "https://api.ipgeolocation.io/ipgeo?apiKey=158cbc50f4774241a285fb74e2043583&ip=" +  Current_Public_IP;
            std::cout<< "Api Url: " << Ip_Geo_Url << std::endl;

            //opens api socket (ip geolocation)
            api.Open_Socket(hostname.c_str(), root_ca_cert_ip, network);
            api.Send_Request(Ip_Geo_Url);
            api.read_Request();
            //parse here
        }
        
        
        



        ThisThread::sleep_for(5000ms);
    }
}