#include "Clienthelper.h"
ClientHelper::ClientHelper()
{
    // decided to develop the TcpClient class. This will contain a connection, SendData and Receive functions.
connection();
while (!toggle)
{ 
    sendData(); 
    receive();
    /* code */
}

   

    close(socketfiledescriptor);
}
void ClientHelper::connection()
{
    socketfiledescriptor = socket(AF_INET, SOCK_STREAM, 0); // normal
    server = gethostbyname("localhost"); // define host
    bzero((char *)&server_address, sizeof(server_address)); // clear server address
    server_address.sin_family = AF_INET; 
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
    // fgets(buffer, 255, stdin);
    cin >> buffer;
    if(!strcmp(buffer,"q"))
    {
        cout <<"quitting" << endl;
        toggle=1;
    }
    if (write(socketfiledescriptor, buffer, strlen(buffer)) < 0)
        perror("ERROR writing to socket");

    bzero(buffer, 256);
}
void ClientHelper::receive()
{
    if (read(socketfiledescriptor, buffer, 255) < 0)
        perror("ERROR reading from socket");
    printf("%s\n", buffer);
}