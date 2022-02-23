#include "Clienthelper.h"
ClientHelper::ClientHelper()
{
    connection();                                  // create the connection
    thread mythread(&ClientHelper::receive, this); // create a receiver thread

    while (!toggle)
    {
        sendData();                                // send data 
    }
    mythread.join();                               // when we exit we join the receiver thread
    close(socketfiledescriptor);                   // close connection
}
void ClientHelper::connection()
{
    socketfiledescriptor = socket(AF_INET, SOCK_STREAM, 0); // normal
    server = gethostbyname("localhost");                    // define host
    bzero((char *)&server_address, sizeof(server_address)); // clear server address
    server_address.sin_family = AF_INET;                    // normal
    bcopy((char *)server->h_addr,
          (char *)&server_address.sin_addr.s_addr,
          server->h_length);
    server_address.sin_port = htons(PORT);
    if (connect(socketfiledescriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        perror("ERROR connecting");
}
void ClientHelper::sendData()
{
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    if (!strcmp(buffer, "quit\n"))
    {
        cout << "quitting" << endl;
        toggle = 1;
    }
    // char str[260];
    // bzero(str, 256);
    // int size = strlen(buffer)-1; // one less since we are writing a space
    // sprintf( str, "%d ", size );
    // strcat(str, buffer);
    // change the message to str......
    
    if (write(socketfiledescriptor, buffer, strlen(buffer)) < 0)
        perror("ERROR writing to socket");

    bzero(buffer, 256);
}
void ClientHelper::receive()
{
    while (!toggle)
    {
        if (read(socketfiledescriptor, buffer, 255) < 0)
            perror("ERROR reading from socket");
        for (long unsigned int i = 0; i < strlen(buffer); i++)
        {
            if (buffer[i] == '\n')
            {
                buffer[i] = ' ';
            }
        }
        printf("%s\n", buffer);
    }
}