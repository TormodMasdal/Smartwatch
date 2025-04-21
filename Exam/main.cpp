#include "mbed.h"
#include "NetworkInterface.h"
#include "Network_Connect.h"
#include <iostream>
#include "API.h"
#include "cerificate.h"
#include "Parse_API.h"


int main() 
{

    Network_Connect network_manager;

    API Ip_Extern_Api;
    API Ip_Geo_Api;

    printf("\nStarting Network Test...\n");

    NetworkInterface* network = network_manager.Connect();

    printf("Network is active!\n");
        
    //Function for fetching external IP
    //For some reason the api calls only works one at a time (fix this)
    //std::string External_IP = Api_Fetch_External_IP(Ip_Extern_Api, network, root_ca_cert_ipify);
    //std::cout<<"External IP: "<<External_IP<<std::endl;
    //Function for testingg internet connection
    Test_Internet_Connection(&network_manager, network);
        
    printf("IpGeoLocation Fetch Starting\n");
       
    //variables for host and url vbeing passed into general api fetch function
    std::string External_IP = "46.212.121.231";
    std::string hostname = "api.ipgeolocation.io";
    std::string Ip_Geo_Url= "https://api.ipgeolocation.io/ipgeo?apiKey=158cbc50f4774241a285fb74e2043583&ip=" +  External_IP;
    std::cout << "Api Url: " << Ip_Geo_Url << std::endl;
    //General API fetch function, fetches ipgeolocation here
    std::string Geo_Response = Api_Fetch(Ip_Geo_Api, hostname, Ip_Geo_Url, root_ca_cert_ipgeo, network);
    Parse_Api_Response(Geo_Response);
    //std::cout<< Geo_Response <<std::endl;
}