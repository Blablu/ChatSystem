/* List of things to be done for the client: 1)Get the IP address of the host 2)Create a socket 3)Establish a connection to the server 4)Send/Receive messages 5)Close the connection to disconnect */
#include <iostream>
#include <string.h>
#include <string>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Importing the library, needed to run WinSock

using namespace std;

int main() {

	// Starting WinSock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2); // Selcting what version of Winsock we want; in this case, we start version 2
	int ws = WSAStartup(ver, &data); // This is where the socket gets started; 

	// Creating a socket
	int sckt = socket(AF_INET, SOCK_STREAM, 0);

	// Setting up a port and an IP address according to the server
	int port = 33000; // Port number
	string ipaddr = "172.20.10.3"; // The test IP; Use "127.0.0.1" as a local IP

	// Pointing the socket to the server and port it has to connect to; In this case, it's set up for an IPv4 addresse only
	sockaddr_in saddr; 
	saddr.sin_family = AF_INET; // Setting the address family; AF_INET is used due to using only an IPv4
	saddr.sin_port = htons(port);
	inet_pton(AF_INET, ipaddr.c_str(), &saddr.sin_addr); // The IP address gets converted from a string to a binery form (numbers) and gets pointed to the socket

	// Connecting to the server
	int cs = connect(sckt, (sockaddr*)&saddr, sizeof(sockaddr_in)); // The actual connectiong of the socket to the server, by using the pointer from above

	// Setting up a string with a size limit which will hold the input text
	string inputText; // A string that will be holding the input of the user
	char buffer[1024]; // A buffer setting the size of the input to 1024 bytes

	// An infinite while loop which runs for as long as there is connection to the server; It takes the input from the user and sends it to the server
	while (true) {
		cout << "Enter your message here: ";
		getline(cin, inputText); // getline is to get the entire line; When spacebar is pressed, the input doesn't get sent as with a cin; This way the user can have blank spaces in their messege
		int sendText = send(sckt, inputText.c_str(), inputText.size(), 0); // Sending the messege on the socket; The input gets converted to 
		int serverResponse = recv(sckt, buffer, 1024, 0); // Receives the messeges from the server on to the socket; The messeges are within the same buffer size - 1024 bytes
		cout << "Message from server: " << string(buffer, 0, serverResponse) << endl; // Messege echo; This way the user sees the messege that the server recieved
	}

	closesocket(sckt); // Closing the socket

	return 0;
}