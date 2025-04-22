#ifndef PARSE_API_H
#define PARSE_API_H
#include <iostream>
#include "json.hpp"

// Parsering av all data fra API
class Parse_API
{
    public:
    std::string Epoch_Parse();
    std::string Longitude_Parse();
    std::string Latitude_Parse();
    std::string City_Parse();
    std::string Date_Time_Parse();
    std::string Weather_Std_Parse();
    std::string Weather_Search_Parse();
    std::string BBC_Parse();
    
    std::string JSON;
    void Parse_Api_Response(std::string api_response);
};

#endif

//Function For Parsing the external IP
std::string Parse_External_IP(std::string response);
//function for removing http body
std::string Dechunk_Http_Body(const std::string& body);
//function for parsing api fetch
//std::string Parse_Api_Response(std::string api_response); 