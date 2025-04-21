#ifndef PARSE_API_H
#define PARSE_API_H
#include <iostream>
#include "json.hpp"

// Parsering av all data fra API
class Parse_API
{
    int Epoch_Parse(char http_response[]);
    float Longitude_Parse(char http_response[]);
    float Latitude_Parse(char http_response[]);
    std::string Date_Time_Parse();
    std::string Weather_Std_Parse();
    std::string Weather_Search_Parse();
    std::string BBC_Parse();
    std::string City_Parse();
};

#endif

//Function For Parsing the external IP
std::string Parse_External_IP(std::string response);
//function for removing http body
std::string Dechunk_Http_Body(const std::string& body);
//function for parsing api fetch
void Parse_Api_Response(std::string api_response); 