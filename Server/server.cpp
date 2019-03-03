#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <thread>
#include <pthread.h>
#include <netinet/in.h>
#include <CoreFoundation/CoreFoundation.h>

using namespace std;

string inputText;
char buff[512];
int clientSocket;
long bytesRecv;
sockaddr_in address;
//CFSocketRef clientSocket;

void msgHandler(int msgSock = socket(AF_INET, SOCK_STREAM, 0))
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    clientSocket = msgSock;

    do
    {
        //memset(buff, 0, 1024);

        //wait for message
        bytesRecv = recv(clientSocket, buff, 5096, 0);

        if (bytesRecv == -1)
        {
            cerr << "There was a connection issue" << endl;
            break;
        }

        if (bytesRecv == 0)
        {
            cout << "The client disconnected" << endl;
            break;
        }
        else
        {
            cout << "Received: " << string(buff, 0, bytesRecv + sizeof(buff)) << endl;
        }
        //resend/echo message

        send(clientSocket, buff, bytesRecv + sizeof(buff), 0);
    }
    while (bytesRecv > 0);
}

int clientHandler()
{

    //create the socket
    int socked = socket(AF_INET, SOCK_STREAM, 0);

    if (socked < 0)
    {
        cerr << "Can't create a socket!";
        return -1;
    }
    
    memset(&address, 0, sizeof(address));
    
    sockaddr_in address;
    address.sin_family = AF_INET;   //use IPv4
    address.sin_port = htons(33000);  //host byte order to neework byte order
    inet_pton(AF_INET, "0.0.0.0", &address.sin_addr);
    
    //bind the IP and port to the socket and error handling in case the binding fails
    if (::bind(socked, (struct sockaddr*)&address, sizeof(address))< 0)
    {
        cerr << "Can't bind to IP/port";
        return -2;
    }
    if (listen(socked, SOMAXCONN) < 0)
    {
        cerr << "Can't listen!";
        return -3;
    }


    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    memset(host, 0, NI_MAXHOST); //gets the numeric form of the host name
    memset(svc, 0, NI_MAXSERV); //get the service info from client
    
    //trying to get the host name and service info from the client
    //(it worked in Code::Blocks in the beginning, but after switching to Xcode id doesn't work anymore)
    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if (result == 0)
    {
        cout << host << " connected on " << svc << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on " << ntohs(client.sin_port) << endl;
    }
    
    //accepting the client
    while (true)
    {
        clientSocket = accept(socked, (struct sockaddr*)&client, &clientSize);
        if (clientSocket < 0)
        {
            cerr << "Problem with client connecting!";
            return -4;
        }
        ::thread cmsgHandler = thread(msgHandler, clientSocket);
        cmsgHandler.detach();
    }
    return 0;
}

int main()
{
    thread startClientHandler(clientHandler);
    startClientHandler.join();


     /*send(clientSocket, "Hello there! ", 12, 0);
     bzero(buff, 512);
     long r = read(clientSocket, buff, 255);
     if (r < 0)
     {
     cerr << "Error reading from socket! ";
     return -5;
     }
     else
     {
     cout << "Success! \n" << buff;
     //r = write(clientSocket, "Confirmed! ", 11);
     }
     //close the socket


     //cout<< "Enter your message here: ";
     //getline(cin, inputText);
     //int sendText = send(clientSocket, inputText.c_str(), inputText.size(), 0);
     //display message, echo message*/


    //close socket
    return 0;

}
