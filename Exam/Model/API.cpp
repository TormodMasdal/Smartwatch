#include "API.h"
#include "mbed.h"

// PARAMETERS: WEBSITE NAME, CERTIFICATE
void API::Open_Socket(const char Web_Name[], const char SSL_CA_PEM[], NetworkInterface* network){


    nsapi_size_or_error_t result;

    // OPEN SOCKET
    result = socket.open(network);
    if(result != 0){
        printf("Failed to open socket %d\n", result);
        while (true) {
            ThisThread::sleep_for(500ms);
        }
    } else {
        printf("Socket open\n");
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
    result = socket.connect(remote_address);
    if(result != 0){
        printf("Failed to connect to serveR %d\n", result);
        while(true){
            ThisThread::sleep_for(500ms);
        }
    } else {
        printf("Connected to server\n");
    }
}


void API::Send_Request(const char http_request[]){
    
    nsapi_size_t total_request_length = strlen(http_request);
    nsapi_size_t offset = 0;
    nsapi_size_or_error_t sent_bytes = 0;

    printf("\nSending message:\n%s", http_request);

    // LOOP: UNTIL ENTIRE REQUEST IS SENT
    while (offset < total_request_length) {
        sent_bytes = socket.send(http_request + offset, total_request_length - offset);
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


void API::read_Request(){

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
        printf("\nThe complete HTTP GET response:\n%s\n", http_response);
    }
}

