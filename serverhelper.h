/**
 * This is the ServerHelper class that will aid helping the server do what it does  ..
 *
 * @author Aaron Bading
 * @date 02/17/22
 * @info Course COP4635
 */

#ifndef ServerHelper_H
#define ServerHelper_H
#include <iostream>
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
using namespace std;

#define PORT 60049 // defining a  port in range 60001 – 60099

class ServerHelper
{
public:
    ServerHelper();                                     /*** Initial setup consturctor.*/
    void start();
void sendresponse(void *message, int msglen , int created_socket);
void communicate();

private:
    int server_filedescriptor, created_socket;  
    long readvalue;
    struct sockaddr_in mysocketaddress , clientaddress; // socket address struck defined in in.h
    int addresslength = sizeof(mysocketaddress);
    int val=1;
};
#endif
