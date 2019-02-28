#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

int main()
{

    //create a socket
    int socked = socket(AF_INET, SOCK_STREAM, 0);
    if (socked == -1)
    {
        cerr << "Can't create a socket!";
        return -1;
    }


    //bind the socket to a port/IP
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(33000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(socked, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        cerr << "Can't bint to IP/port";
        return -2;
    }

    if (listen(socked, SOMAXCONN) == -1)
    {
        cerr << "Can't listen!";
        return -3;
    }

    //accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(socked, (sockaddr*)&client, &clientSize);
    if (clientSocket == -1)
    {
        cerr << "Problem with client connecting!";
        return -4;
    }


    //close the socket
    close(socked);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if (result)
    {
        cout << host << "connected on " << svc << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << "connected on " << ntohs(client.sin_port) << endl;
    }


    //display message, echo message
    char buff[4096];
    while (true)
    {
        //clear the buffer
        memset(buff, 0, 4096);

        //wait for message
        int bytesRecv = recv(clientSocket, buff, 4096, 0);

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

        //display message
        cout << "Received: " << string(buff, 0, bytesRecv) << endl;

        //resend message
        send(clientSocket, buff, bytesRecv +1, 0);

    }
    //close socket
    close(clientSocket);

    return 0;
}
