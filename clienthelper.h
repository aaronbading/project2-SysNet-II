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

using namespace std;

#define PORT 60049 // defining a  port in range 60001 – 60099

class ClientHelper
{
public:
    ClientHelper(); /*** Initial setup consturctor.*/
    void connection();
    void sendData();
    void receive();
private:
    int socketfiledescriptor, n;
    struct sockaddr_in server_address;
    struct hostent *server;
    char buffer[256];
    int toggle =0;
};
#endif
