#include "Parse_API.h"

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
void Parse_Api_Response(std::string api_response) 
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

    //printf("Parsed JSON:\n%s\n", parsed.dump(2).c_str());
    //country
    std::string country = parsed.value("country_name", "Unknown country");
    std::cout << "Country: " << country << std::endl;

    //city
    std::string city = parsed.value("city", "Uknown City");
    std::cout<< "City: " << city << "\n";

    //datetime
    std::string datetime = parsed["time_zone"].value("current_time", "Unknown time");
    std::cout << "Current time: " << datetime << "\n";

}
