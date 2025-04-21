#include "API.h"
#include "mbed.h"
#include "Parse_API.h"


void API::Open_Socket(const char Web_Name[], const char SSL_CA_PEM[], NetworkInterface* network)
{

    int retries = 3;
    nsapi_size_or_error_t result;

    // OPEN SOCKET
   while (retries--) {
        result = socket.open(network);
        if (result == NSAPI_ERROR_OK) break;
        printf("Socket open failed (retries left: %d)\n", retries);
        ThisThread::sleep_for(2000ms);
    }
    if (result != NSAPI_ERROR_OK) {
        printf("Fatal: Could not open socket\n");
        return;
    }

    // RESOLVE DNS FOR WEBSITE
    SocketAddress remote_address;
    result = network->gethostbyname(Web_Name, &remote_address);
    if(result != 0){
        printf("Failed to resolve host %d\n", result);
        while(true){
            ThisThread::sleep_for(500ms);
        }
    } else {
        printf("Resolved host & Acquired IP: %s\n", remote_address.get_ip_address());
    }

    // SET CERTIFICATE FOR TLS
    result = socket.set_root_ca_cert(SSL_CA_PEM);
    if(result != 0){
        printf("Failed to validate CA Certificate %d\n", result);
        while(true){
            ThisThread::sleep_for(500ms);
        }
    } else {
        printf("CA Certificate Validated\n");
    }
    


    // SET HOSTNAME & CONNECT TO SERVER
    socket.set_hostname(Web_Name);
    remote_address.set_port(443);

    int max_retries = 5;
    int retry_delay_ms = 3000; // 2 seconds between retries
    nsapi_size_or_error_t result2;

    for (int attempt = 1; attempt <= max_retries; attempt++) 
    {
    result = socket.connect(remote_address);

    if (result == NSAPI_ERROR_OK) {
        printf("Connected to server (attempt %d/%d)\n", attempt, max_retries);
        break; // Success! Exit retry loop.
    }

    // Log failure (with error code meanings)
    const char* error_msg = "Unknown error";
    switch (result) {
        case NSAPI_ERROR_NO_CONNECTION:  error_msg = "No network"; break;
        case NSAPI_ERROR_DNS_FAILURE:    error_msg = "DNS failed"; break;
        case NSAPI_ERROR_TIMEOUT:        error_msg = "Timeout"; break;
        case NSAPI_ERROR_AUTH_FAILURE:   error_msg = "TLS handshake failed"; break;
    }
    printf("Connection failed (attempt %d/%d): %d (%s)\n", attempt, max_retries, result, error_msg);

    if (attempt < max_retries) {
        printf("Retrying in %dms...\n", retry_delay_ms);
        ThisThread::sleep_for(retry_delay_ms);
    } else {
        printf("Max retries reached. Aborting.\n");
        return; // Or handle fatal error (e.g., reboot)
    }
    }
}


void API::Send_Request(const std::string& url) 
{
    // Extract host and path
    size_t proto_pos = url.find("://");
    size_t host_start = (proto_pos != std::string::npos) ? proto_pos + 3 : 0;
    size_t path_pos = url.find("/", host_start);
    
    std::string host = url.substr(host_start, path_pos - host_start);
    std::string path = (path_pos != std::string::npos) ? url.substr(path_pos) : "/";

    // Construct the request
    std::string http_request = 
        "GET " + path + " HTTP/1.1\r\n"
        "Host: " + host + "\r\n"
        "Connection: close\r\n"
        "User-Agent: MbedClient/1.0\r\n"
        "Accept: */*\r\n"
        "\r\n";

    nsapi_size_t total_request_length = http_request.length();
    nsapi_size_t offset = 0;
    nsapi_size_or_error_t sent_bytes = 0;

    printf("\nSending message:\n%s", http_request.c_str());

    while (offset < total_request_length) {
        sent_bytes = socket.send(http_request.c_str() + offset, total_request_length - offset);
        if (sent_bytes < 0) {
            printf("Failed to send HTTP request: %d\n", sent_bytes);
            break;
        }
        printf("Sent %d bytes\n", sent_bytes);
        offset += sent_bytes;
    }

    if (offset == total_request_length) {
        printf("Complete message sent\n");
    }
}


std::string API::Read_Request()
{

    // RECIEVE HTTP RESPONSE
    static constexpr size_t HTTP_RESPONSE_BUF_SIZE = 4000;
    static char http_response[HTTP_RESPONSE_BUF_SIZE + 1];
    memset(http_response, 0, sizeof(http_response));

    nsapi_size_t remaining_bytes = HTTP_RESPONSE_BUF_SIZE;
    nsapi_size_or_error_t received_bytes = 0;
    nsapi_size_t total_received = 0;

    std::string temp;

    // LOOP: TO READ RESPONSE
    while (remaining_bytes > 0) {
        nsapi_size_or_error_t result = socket.recv(http_response + total_received, remaining_bytes);
        if (result < 0) {
            received_bytes = result;
            break;
        }
        if (result == 0) {
            break;
        }
        printf("Received %d bytes\n", result);
        
        total_received += result;
        remaining_bytes -= result;
        
    }

    if (total_received < 0) {
        printf("Failed to read the HTTP response: %d\n", total_received);
    } else {
        http_response[total_received] = '\0';
       // printf("\nThe complete HTTP GET response:\n%s\n", http_response);
    }
    return http_response;
}


void API::Close_Socket()
{
    socket.close();
}

//FUnction for fetching your external IP
std::string Api_Fetch_External_IP(API& Api, NetworkInterface * network, const char root_ca_cert[]) 
{        
    //Fetches External IP
    //variables for host and url
    std::string Ipify = "api.ipify.org";
    std::string IpifyURL = "https://api.ipify.org/";
    //Opens the socket
    Api.Open_Socket( Ipify.c_str(), root_ca_cert , network);
    //Sends Request
    Api.Send_Request(IpifyURL);
    //Stores result
    std::string IP_Extern_Api_Response = Api.Read_Request();
    //Closes the socket
    Api.Close_Socket();
    //Parses the result so that were left with only IP
    std::string External_IP = Parse_External_IP(IP_Extern_Api_Response);
    ThisThread::sleep_for(500ms);
    //disconnects then connects (resets the NIC hopefully)
    return External_IP;
}
    
//Function for a general API fetch
std::string Api_Fetch(API& Api, std::string hostname, std::string url, const char root_ca_cert[], NetworkInterface* network)
    {
    //Sleeps a little between fetches
    ThisThread::sleep_for(500);
    //Opens api socket (ip geolocation)
    Api.Open_Socket(hostname.c_str(), root_ca_cert, network);
    //Sends request
    Api.Send_Request(url);
    //Reads Request and stores in string
    std::string Read_Api = Api.Read_Request();
    //Closes the socket and sleeps
    Api.Close_Socket();
    ThisThread::sleep_for(1000ms);
    return Read_Api;
    }