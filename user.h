/**
 * This is the User class
 *
 * @author Aaron Bading
 * @date 02/24/22
 * @info Course COP4635
 */

#ifndef User_H
#define User_H
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

// Username
// Locations
// SocketNo
// Messages ... ?? messages that it is supposed to receive ?? for private messages/ group messages ??
class User
{
public:
    User(string username, int socketnumber); /*** Initial setup consturctor.*/
    string getusername();
    int getsocketnumber();
    vector<string> getlocations();
    void appendlocation(string location);
    void removelocation(string thelocation);

private:
    string username;
    vector<string> locations;
    int socketnumber;
    vector<char *> messages;
};
#endif
