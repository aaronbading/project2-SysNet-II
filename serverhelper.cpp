#include "serverhelper.h"
#include <string.h>
ServerHelper::ServerHelper()
{
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

}

void ServerHelper::start()
{
    cout << "Waiting for Incoming connections " << endl;
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

        if ((mytempsocket = accept(server_filedescriptor, (struct sockaddr *)&mysocketaddress, (socklen_t *)&addresslength)) < 0)
        {
            perror("Error occured in accept");
            exit(EXIT_FAILURE);
        }
        thread mythread (&ServerHelper::acceptUser, this);
        mythread.detach();

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

void ServerHelper::statechange(char *message)
{
}
void ServerHelper::acceptUser()
{// thread function 
    int thesocket = this->mytempsocket;
    char receivedmessage[56];
    char mymessage[50];
    bzero(mymessage, 56);
    strcpy(mymessage, "Server received message\n\n");
    
    DisplayMenu(thesocket,0);

    while (strcmp(receivedmessage, "quit\n"))
    { // while exit hasnt been typed in
        bzero(receivedmessage, 56);
        if (read(thesocket, receivedmessage, 56) < 0)
        {
            perror("Error occured in reading");
        }

        // send(thesocket, receivedmessage, strlen(receivedmessage)+1, 0); // echo the message
        cout << "Received message from socket :  " << thesocket  << " is " << receivedmessage << endl;
        //send(thesocket, mymessage, strlen(mymessage) + 1, 0);
        
        DisplayMenu(thesocket,1);
        statechange(receivedmessage);
    }

    cout << "at the end..." << endl;
    close(thesocket);
}
// Display Menu function called by threads .. takes in temp which is the socket and choice .. 
void ServerHelper::DisplayMenu(int temp, int choice)
{
    bzero(loginmenu, 82);
    strcpy(loginmenu, "Welcome! \n\n Press 1 to Login \n Press 2 to Register \n Type 'quit' to Quit ");

    switch (choice)
    {
    case 0://welcome message
        send(temp, "Connected , Press Enter To Continue\n",38,0);
        break;
    case 1:
        send(temp, loginmenu, 82,0);
        break;
    default:
        break;
    }

}

