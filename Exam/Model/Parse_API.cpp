#include "Parse_API.h"
#include "API.h"
#include <string>

//Function For Parsing the external IP
using json = nlohmann::json;


//function for parsing the external ip
std::string Parse_External_IP(std::string response)
{

    size_t ip_start = response.find_last_not_of(" \t\n\r");

    if (ip_start != std::string::npos) 
    {
        response.resize(ip_start + 1);
    }

    
    size_t body_pos = response.find("\r\n\r\n");
    if (body_pos != std::string::npos) 
    {
        return response.substr(body_pos + 4); 
    }
    return response;
}

//function that removes the http body
std::string Dechunk_Http_Body(const std::string& body) {
    std::string output;
    size_t pos = 0;

    while (pos < body.length()) {
        size_t line_end = body.find("\r\n", pos);
        if (line_end == std::string::npos) break;

        std::string chunk_size_str = body.substr(pos, line_end - pos);
        size_t chunk_size = std::stoul(chunk_size_str, nullptr, 16);  // HEX

        if (chunk_size == 0) break;

        pos = line_end + 2; // skip \r\n
        output += body.substr(pos, chunk_size);
        pos += chunk_size + 2; // skip chunk + \r\n
    }

    return output;
}

//function for parsing api response with json
void Parse_API::Parse_Api_Response(std::string api_response) 
{
    
    //Separate headers and body
    size_t header_end = api_response.find("\r\n\r\n");
    if (header_end == std::string::npos) 
    {
        printf("Malformed HTTP response\n");
        return;
    }

    std::string body = api_response.substr(header_end + 4);

    //dechunk
    bool is_chunked = api_response.find("Transfer-Encoding: chunked") != std::string::npos;
    if (is_chunked) 
    {
        body = Dechunk_Http_Body(body);
    }
    // Parse without throwing
     json parsed = json::parse(body, nullptr, false);
    if (parsed.is_discarded()) 
    {
        printf("Failed to parse JSON.\n");
        return;
    }

    // Prints entire JSON
    //printf("Parsed JSON:\n%s\n", parsed.dump(2).c_str());

    JSON = parsed.dump(2).c_str();
}

// Parse unix epoch time
std::string Parse_API::Epoch_Parse(){
    json parsed = json::parse(JSON);

    // Search for current_time_unix in object time_zone
    double unix = parsed["time_zone"].value("current_time_unix", double());

    //Convertes double to string, makes the entire epoch time readable
    std::string Epoch = std::to_string(unix);
    std::cout << "Unix epoch time: " << Epoch << std::endl;
    return Epoch;
}

// Parse longitude
std::string Parse_API::Longitude_Parse(){
    json parsed = json::parse(JSON);
    std::string longitude = parsed.value("longitude", "Unknown longitude");
    std::cout << "Longitude: " << longitude << std::endl;
    return longitude;
}

// Parse latitude
std::string Parse_API::Latitude_Parse(){
    json parsed = json::parse(JSON);
    std::string latitude = parsed.value("latitude", "Unknown latitude");
    std::cout << "Latitude: " << latitude << std::endl;
    return latitude;
}

// Parse city
std::string Parse_API::City_Parse(){
    json parsed = json::parse(JSON);
    std::string city = parsed.value("city", "Unknown city");
    std::cout << "City: " << city << std::endl;
    return city;
}


