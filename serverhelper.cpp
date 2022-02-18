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
    // listen tells the socket to listen for incoming connections
    // it places incoming connections into a backlog queue of size 10 
    //until we call accept()
    if (listen(server_filedescriptor, 10) < 0)
    {
        perror("Error occured in listen");
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
    char buffer[256];
    int i = 1;
    while (i)
    {
       
         if ((created_socket = accept(server_filedescriptor, (struct sockaddr *)&mysocketaddress, (socklen_t *)&addresslength)) < 0)
        {
            perror("Error occured in accept");
            exit(EXIT_FAILURE);
        }
        
        bzero(buffer, 256);
        send(created_socket, "Hello, world!\n", 13, 0);
        //
       //sendresponse("Hello, world!\n",13,created_socket);
        if(read(created_socket, buffer, 255) < 0)
        {
            perror("Error occured in reading");
        }
        printf("Here is the message: %s\n", buffer);
        if(fork() == 0)
        {
            communicate();
        }   
    
    }

}
void ServerHelper::sendresponse(void *message, int msglen , int created_socket)
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
void ServerHelper::communicate()
{

}