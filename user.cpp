#include "user.h"
User::User(string username, int socketnumber)
{
    this->username = username;
    this->socketnumber = socketnumber;
}
string User::getusername()
{
    return username;
}
int User::getsocketnumber()
{
    return socketnumber;
}
vector<string> User::getlocations()
{
    return locations;
}
void User::appendlocation(string location)
{
locations.push_back(location);
}
void User::removelocation(string thelocation)
{

for (long unsigned int i = 0; i < locations.size(); i++)
{
    if(locations.at(i) == thelocation)
    {
        locations.erase(locations.begin()+i);
    }
}

}