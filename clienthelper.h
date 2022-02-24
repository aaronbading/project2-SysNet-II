/**
 * This is the ClientHelper class that will aid helping the client do what it does  ..
 *
 * @author Aaron Bading
 * @date 02/17/22
 * @info Course COP4635
 */

#ifndef ClientHelper_H
#define ClientHelper_H
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <netdb.h>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

#define PORT 60049                  // defining a  port in range 60001 – 60099
#define inputoutputbuffersize 500   // defining a  port in range 60001 – 60099

class ClientHelper
{
public:
    ClientHelper();         /*** Initial setup consturctor.*/
    void connection();      /*** Create the connection && this is where host name is defined as localhost*/
    void sendData();        /*** Sends Data To server*/
    void receive();          /*** Receives Data from server*/
private:
    int socketfiledescriptor, n;
    struct sockaddr_in server_address;
    struct hostent *server;
    char inputbuffer[inputoutputbuffersize];
    char outputbuffer[inputoutputbuffersize];
    int toggle;
};
#endif
