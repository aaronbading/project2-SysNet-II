#include "Clienthelper.h"
ClientHelper::ClientHelper()
{
    toggle =0;
    connection();                                  // create the connection
    thread mythread(&ClientHelper::receive, this); // create a receiver thread

    //****//
    //     bzero(inputbuffer, 500);
    //  if (read(socketfiledescriptor, inputbuffer, 500) < 0)
    //         perror("ERROR reading from socket");
    //     printf("%s\n", inputbuffer);
    //***///
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
    bzero(outputbuffer, 500);
    fgets(outputbuffer, 500, stdin);

    if (!strcmp(outputbuffer, "quit\n"))
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

    if (write(socketfiledescriptor, outputbuffer, strlen(outputbuffer)) < 0)
        perror("ERROR writing to socket");

}
void ClientHelper::receive()
{
    while (!toggle)
    {
        if (read(socketfiledescriptor, inputbuffer, 500) < 0)
            perror("ERROR reading from socket");
        printf("%s\n", inputbuffer);
        bzero(inputbuffer, 500);

    }
}