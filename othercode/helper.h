/**
 * This is the Helper class that will aid in doing the servers tasks ..
 *
 * @author Aaron Bading
 * @date 02/11/22
 * @info Course COP4635
 */

#ifndef Helper_H
#define Helper_H
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
class Helper
{
public:
    Helper();                                     /*** Initial setup consturctor.*/
    void start();                                 /*** This gets the process going and starts listening for requests*/
    void interpretRequest(char *input);           /*** This function takes in the request and processes it*/
    void readfiles();                             /*** This function aids in reading the html files*/
    void createMessage(char *myfile, char *type); /*** This function assembles the header that will be sent*/
    void sendresponse(void *message, int msglen); /*** This function sends the to the socket*/
    void sendpicture(char *image_path);            /*** This function sends the image*/

private:
    int server_filedescriptor, created_socket;  
    long readvalue;
    struct sockaddr_in mysocketaddress; // socket address struck defined in in.h
    int addresslength = sizeof(mysocketaddress);
    char header[240];
    char indexfile[490], notfoundfile[490], picturefile[490], defaultfile[490];
};
#endif
