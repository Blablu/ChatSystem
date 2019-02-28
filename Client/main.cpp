/* List of things to be done for the client: 1)Get the IP address of the host 2)Create a socket 3)Establish a connection to the server 4)Send/Receive messages 5)Close the connection to disconnect */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(){

    // Setting up a port and an IP address
    int port = ; // ADD WHEN SERVER IS READY
    string ipaddr = "127.0.0.1" // LOCAL IP/IP OF THIS MACHINE

    sockaddr_in saddr; // For IPv4 addresses
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    inet_pton(AF_INET, ipaddr.c_str(), &saddr.sin_addr);

    // Crating a socket
    int sckt = socket(AF_INET, SOCK_STREAM, 0);

    // Connecting to the server
    int cs = connect(sckt, (sockaddr*)&saddr, sockaddr_in);

    // Setting up a string with a size limit, which will hold the input text
    string inputText;
    char buffer[1024];

    // While loop for sending the messages to the server
    while(true){
            cout << "Enter your message here: ";
            cin >> inputText;
            int sendText = send(sckt, textInput.c_str(), textInput.size(), 0);
            int serverResponse = recv(sckt, buffer, 1024, 0);
            cout << "Message from server: " << string(buffer, serverResponse)<< "\n";
    }
    close(sckt);

    return 0;
}

