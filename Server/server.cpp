#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <pthread.h>


using namespace std;

void *task1(void *);

static int connFd;

void *task1 (void *dummyPt)
{
    cout << "Thread No: " << pthread_self() << endl;
    char test[300];
    bzero(test, 301);
    bool loop = false;
    while(!loop)
    {
        bzero(test, 301);
        read(connFd, test, 300);
        string tester (test);
        cout << tester << endl;
        if (tester == "exit")
            break;
    }
    cout << "Close thread " << endl;
    close(connFd);
}

int main()
{
    string inputText;
    char buff[256];
    int clientSocket;

    pthread_t threadA[3];

    //create a socket
    int socked = socket(AF_INET, SOCK_STREAM, 0);
    if (socked < 0)
    {
        cerr << "Can't create a socket!";
        return -1;
    }


    //bind the socket to a port/IP
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(33000);  //host byte order to neework byte order
    inet_pton(AF_INET, "0.0.0.0", &address.sin_addr);

    if (bind(socked, (struct sockaddr*)&address, sizeof(address))< 0)
    {
        cerr << "Can't bind to IP/port";
        return -2;
    }

    if (listen(socked, 5) < 0)
    {
        cerr << "Can't listen!";
        return -3;
    }

    int noThread = 0;


    //accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    while (noThread < 3)
    {

    clientSocket = accept(socked, (struct sockaddr*)&client, &clientSize);
    if (clientSocket < 0)
    {
        cerr << "Problem with client connecting!";
        return -4;
    }

    //close(socked);

    pthread_create(&threadA[noThread], NULL, task1, NULL);
    noThread++;
    }

    for(int i = 0; i < 3; i++)
    {
        pthread_join(threadA[i], NULL);
    }

    send(clientSocket, "Hello there! ", 12, 0);

    bzero(buff, 256);

    int r = read(clientSocket, buff, 255);
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


    memset(host, 0, NI_MAXHOST); //gets the numeric form of the host name
    memset(svc, 0, NI_MAXSERV); //get the service info from client

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if (result)
    {
        cout << host << " connected on " << svc << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on " << ntohs(client.sin_port) << endl;
    }

    //cout<< "Enter your message here: ";
    //getline(cin, inputText);
    //int sendText = send(clientSocket, inputText.c_str(), inputText.size(), 0);


    //display message, echo message

    while (true)
    {
        //clear the buffer
        memset(buff, 0, 1024);

        //wait for message
        int bytesRecv = recv(clientSocket, buff, 1024, 0);

        if (bytesRecv < 0)
        {
            cerr << "There was a connection issue" << endl;
            break;
        }

        if (bytesRecv = 0)
        {
            cout << "The client disconnected" << endl;
            break;
        }

        //display message
        cout << "Received: " << string(buff, 0, bytesRecv + sizeof(buff)) << endl;

        //resend message

        send(clientSocket, buff, bytesRecv + sizeof(buff), 0);

    }
    //close socket
    close(clientSocket);

    return 0;
}
