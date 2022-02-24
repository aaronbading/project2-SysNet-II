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
#include <ctype.h>
//#include <sys/types.h>
//#include <fcntl.h>
//#include <sys/sendfile.h>
#include <sys/stat.h>
#include <thread>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <sstream>
#include <fstream>
#include <cstring>
#include "user.h"
using namespace std;

#define PORT 60049 // defining a  port in range 60001 – 60099
#define loggedinstate 3
#define registerstate 2
class ServerHelper
{
public:
    ServerHelper();                                                   /*** Initial setup consturctor.*/
    void start();                                                     // start the process of
    void sendresponse(void *message, int msglen, int created_socket); // send a response to client
    int statechange(char *message, int mysocket);                     // change the state of the system ..
    void acceptUser();                                                // thread function that is called by thread
    void DisplayMenu(int temp, int choice);                           // depending on what choice is , it will display menu
    bool usernamecheck(string username);                              // check if username is present
    bool usernamepasswordcheck(string username, string password);   
    bool passwordcheck(string password);
    void loggedinstatechange(char *message, int mysocket);

private:
    int server_filedescriptor, created_socket;
    long readvalue;
    struct sockaddr_in mysocketaddress, clientaddress; // socket address struck defined in in.h
    int addresslength = sizeof(mysocketaddress);

    char loginmenu[82];     // menu display for logging/ registration
    char connectedmenu[38]; // menu display for connection
    char newlinemessage[1];
    char loggedinmenu[174]; // menu display for being logged in
    int mytempsocket;       // temporary container for socket id
    vector<User> myusers;
};
#endif
