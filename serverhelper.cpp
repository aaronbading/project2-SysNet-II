#include "serverhelper.h"
#include <string.h>
ServerHelper::ServerHelper()
{

    // int sockfd, newsockfd, portno;
    // socklen_t clilen;
    // char buffer[256];
    // struct sockaddr_in serv_addr, cli_addr;
    // int n;
    if ((server_filedescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error occured in socket");
        exit(EXIT_FAILURE);
    }

    // clear address structure
    bzero((char *)&mysocketaddress, sizeof(mysocketaddress));

    /* host_addr structure for bind call is being set up  */
    // server byte order
    mysocketaddress.sin_family = AF_INET;

    //  fill current host's IP address
    mysocketaddress.sin_addr.s_addr = INADDR_ANY;

    // converting a short integer value for port into network byte order
    mysocketaddress.sin_port = htons(PORT);
    memset(mysocketaddress.sin_zero, '\0', sizeof(mysocketaddress.sin_zero));

    // bind process parameters are the socket , socketaddress and the size of socketaddress
    if (bind(server_filedescriptor, (struct sockaddr *)&mysocketaddress, sizeof(mysocketaddress)) < 0)
    {
        perror("Error occured in bind");
        exit(EXIT_FAILURE);
    }

    start(); // move forward

    // send(created_socket, "Hello, world!\n", 13, 0);
    // char buffer[256];
    // int n;
    // bzero(buffer, 256);

    // if (read(created_socket, buffer, 255) < 0)
    //     perror("ERROR reading from socket");
    // printf("Here is the message: %s\n", buffer);

    // close(created_socket);
    // close(server_filedescriptor);
}
void ServerHelper::start()
{
    vector<thread> mythreads;

    while (1)
    {
        // listen tells the socket to listen for incoming connections
        // it places incoming connections into a backlog queue of size 10
        // until we call accept()
        if (listen(server_filedescriptor, 10) < 0)
        {
            perror("Error occured in listen");
            exit(EXIT_FAILURE);
        }

        // thread th1(acceptUser());
        // mythreads.push_back(th1);
        // for(thread & th : mythreads)
        // {
        //     if(th.joinable())
        //     th.join();
        // }

        // int tempsocket;
        // if ((tempsocket = accept(server_filedescriptor, (struct sockaddr *)&mysocketaddress, (socklen_t *)&addresslength)) < 0)
        // {
        //     perror("Error occured in accept");
        //     exit(EXIT_FAILURE);
        // }
        // printf("forking");
        // if (fork() == 0)
        // {
        //     acceptUser(tempsocket);
        //     exit(EXIT_SUCCESS);
        // }
        // create thread that calls that function
        cout << " before accepting " << endl;
        int tempsocket;
        if ((tempsocket = accept(server_filedescriptor, (struct sockaddr *)&mysocketaddress, (socklen_t *)&addresslength)) < 0)
        {
            perror("Error occured in accept");
            exit(EXIT_FAILURE);
        }
        cout << "after accepting " << endl;
        if(fork() == 0)
        {
            acceptUser(tempsocket);
            cout << "CHILD EXITS" << endl;
            exit(EXIT_SUCCESS);
        }
    cout << "PARENT after forking now im ready again " << endl;

    }
    close(server_filedescriptor);
}
void ServerHelper::sendresponse(void *message, int msglen, int created_socket)
{
    char *msg = (char *)message;

    while (msglen > 0)
    {
        int len = write(created_socket, msg, msglen);
        if (len <= 0)
            return;
        msg += len;
        msglen -= len;
    }
}

void ServerHelper::acceptUser(int thesocket)
{
    // seperate process  just called this
    char themessage[256];
    char mymessage[50];
    bzero(mymessage, 256);
    bzero(themessage, 256);
    strcpy(mymessage,"Server received message");
    cout << " CHILD inside accept User" << endl;
    // sendresponse("Hello, world!\n",13,created_socket);
    while (strcmp(themessage, "quit"))
    { // while exit hasnt been typed in
            bzero(themessage, 256);
            if (recv(thesocket, themessage, 255,0) < 0)
            {
                perror("Error occured in reading");
            }
        
           // send(thesocket, themessage, strlen(themessage)+1, 0);
          cout << "Money : " << themessage << endl; 
          send(thesocket, mymessage, strlen(mymessage) + 1, 0);
         statechange(themessage);
        // communicate(thesocket);
    }
    
    cout << "at the end..." << endl;
    close(thesocket);
}
void ServerHelper::statechange(char* message)
{


}