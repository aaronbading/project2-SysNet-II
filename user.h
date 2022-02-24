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
class User
{
public:
    User(string username, int socketnumber); /*** Initial setup consturctor.***/
    string getusername();                    /*** get username .***/
    int getsocketnumber();                   /*** get socketnumber .***/
    vector<string> getlocations();           /*** return vector of locations ***/
    void appendlocation(string location);    /*** append a location  ***/
    void removelocation(string thelocation); /*** remove a location  ***/

private:
    string username;
    vector<string> locations;
    int socketnumber;
    vector<char*> messages;
};
#endif
