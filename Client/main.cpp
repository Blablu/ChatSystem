/* List of things to be done for the client: 1)Get the IP address of the host 2)Create a socket 3)Establish a connection to the server 4)Send/Receive messages 5)Close the connection to disconnect */
#include <iostream>
#include <string.h>
#include <string>
#include <Ws2tcpip.h>

using namespace std;

int main(){

    // WinSock
    WSADATA data;
    WORD ver = MAKEWORD(2, 2);
    int ws = WSAStartup(ver, &data);

    // Creating a socket
    int sckt = socket(AF_INET, SOCK_STREAM, 0);

    // Setting up a port and an IP address
    int port = 33000; // ADD WHEN SERVER IS READY
    string ipaddr = "170.20.10.5"; // LOCAL IP/IP OF THIS MACHINE

    sockaddr_in saddr; // For IPv4 addresses
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    inet_pton(AF_INET, ipaddr.c_str(), &saddr.sin_addr); // THIS TEXT-TO-BINARY CONVERSION FUNCTION IS NOT WROKING IN CODE::BLOCKS

    // Connecting to the server
    int cs = connect(sckt, (sockaddr*)&saddr, sizeof(sockaddr_in));

    // Setting up a string with a size limit which will hold the input text
    string inputText;
    char buffer[1024];

    // While loop for sending the messages to the server
    while(true){
            cout << "Enter your message here: ";
            getline(cin, inputText);
            int sendText = send(sckt, inputText.c_str(), inputText.size(), 0);
            int serverResponse = recv(sckt, buffer, 1024, 0);
            cout << "Message from server: " << string(buffer, serverResponse)<< "\n";
    }
    close(sckt);

    return 0;
}

