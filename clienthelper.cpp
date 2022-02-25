#include "clienthelper.h"
ClientHelper::ClientHelper()
{
    toggle =0;
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
    bzero(outputbuffer, inputoutputbuffersize);
    fgets(outputbuffer, inputoutputbuffersize, stdin);

    if (!strcmp(outputbuffer, "quit\n"))
    {
        cout << "Bye Bye ! Come again. " << endl;
        toggle = 1;
    }

    if (write(socketfiledescriptor, outputbuffer, strlen(outputbuffer)) < 0)
        perror("ERROR writing to socket");

}
void ClientHelper::receive()
{
    while (!toggle)
    {
        if (read(socketfiledescriptor, inputbuffer, inputoutputbuffersize) < 0)
            perror("ERROR reading from socket");
        printf("%s\n", inputbuffer);
        bzero(inputbuffer, inputoutputbuffersize);

    }
}