#include "serverhelper.h"
#include <string.h>
//TODO   :: change name of the see all the locatiosn you are subscribed to -> of a given user. 
//*************************
// SEND A GROUP MESSAGE     < NEEDS DONE >
// Gather up all locations .. -> go through all online users and gather up all locations
//remove duplicates
//display all locations..
//Get What location the message goes to
//What is the message ?  -> Message   -> pushback(messages)
//Gather all users that have are subscribed to this location ..
// Send message to each one of those users ........ for loop .. 

//*************************
// SEND A PRIVATE MESSAGE   < DONE  >  NOT TESTED
//Display online users ..  < CHECK > 
//Get what user the message is sent to -> Socket Number  < Check > 
//What is the message ?  -> Message   -> pushback(messages)  < check > 
//Send message to relavent SocketNumber  < check >

//*************************
// SEE ALL ONLINE USERS     <  CHECK >
// SEE THE LAST 10 MESSAGES .. < CHECK  >  NOT TESTED
// Simply display the vector of previous messages that has been kept track of . 

//*************************
ServerHelper::ServerHelper()
{
    //** Define some static messages
    bzero(loginmenu, 68);
    strcpy(loginmenu, "\n\n Press 1 to Login \n Press 2 to Register \n Type 'quit' to Quit ");
    bzero(connectedmenu, 38);
    strcpy(connectedmenu, "Connected , Press enter to continue\n");
    bzero(newlinemessage, 1);
    strcpy(newlinemessage, "\n");
    bzero(loggedinmenu, 307);
    strcpy(loggedinmenu,"\nSuccessfully logged in \n 1. Subscribe to a location \n 2. Unsubscribe from a location \n 3. Send a message to a location \n 4. Send a Private Message \n 5. See all the locations you are subscribed to \n 6. See all the online Users \n 7. See last 10 messages \n 8. Change Password \n Type quit to Exit\n\n");
    // change logged in menu message for part II of this more involved project than once thought .

    if ((server_filedescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error occured in socket");
        exit(EXIT_FAILURE);
    }
    // clear address structure
    bzero((char *)&mysocketaddress, sizeof(mysocketaddress));

    /* host_addr structure for bind call is being set up  */
    // server byte order
    mysocketaddress.sin_family = AF_INET;

    //  fill current host's IP address
    mysocketaddress.sin_addr.s_addr = INADDR_ANY;

    // converting a short integer value for port into network byte order
    mysocketaddress.sin_port = htons(PORT);
    memset(mysocketaddress.sin_zero, '\0', sizeof(mysocketaddress.sin_zero));

    // bind process parameters are the socket , socketaddress and the size of socketaddress
    if (bind(server_filedescriptor, (struct sockaddr *)&mysocketaddress, sizeof(mysocketaddress)) < 0)
    {
        perror("Error occured in bind");
        exit(EXIT_FAILURE);
    }

    start(); // move forward
}

void ServerHelper::start()
{
    cout << "Waiting for Incoming connections " << endl;
    while (1)
    {
        // listen tells the socket to listen for incoming connections
        // it places incoming connections into a backlog queue of size 10
        // until we call accept()
        if (listen(server_filedescriptor, 10) < 0)
        {
            perror("Error occured in listen");
            exit(EXIT_FAILURE);
        }

        if ((mytempsocket = accept(server_filedescriptor, (struct sockaddr *)&mysocketaddress, (socklen_t *)&addresslength)) < 0)
        {
            perror("Error occured in accept");
            exit(EXIT_FAILURE);
        }
        thread mythread(&ServerHelper::acceptUser, this);
        mythread.detach(); // this detaches a thread meaning that it does not have to be joined.. the thread gives its resources up automatically
            cout << "Client Connected" << endl;

    }
    close(server_filedescriptor);
}

void ServerHelper::sendresponse(void *message, int msglen, int created_socket)
{ // this is the sendingrespone method/function
    char *msg = (char *)message;

    while (msglen > 0)
    {
        int len = write(created_socket, msg, msglen);
        if (len <= 0)
            return;
        msg += len;
        msglen -= len;
    }
}

// thread function ..
void ServerHelper::acceptUser()
{
    char receivedmessage[56];
    bzero(receivedmessage, 56);
    int state = 2;
    int thesocket = mytempsocket;
    DisplayMenu(thesocket, 1); // display the connected message
    DisplayMenu(thesocket, 2); // display the menu

    while (strcmp(receivedmessage, "quit\n"))
    { // while exit hasnt been typed in
        bzero(receivedmessage, 56);
        if (read(thesocket, receivedmessage, 56) < 0)
        {
            perror("Error occured in reading");
        }

        //cout << "Received message from socket :  " << thesocket << " is " << receivedmessage << endl;

        if (!strcmp(receivedmessage, "quit\n"))
            break;

        if (state == loggedinstate)
        {
            loggedinstatechange(receivedmessage, thesocket);
            DisplayMenu(thesocket, loggedinstate);
        }
        else
        {
            state = statechange(receivedmessage, thesocket);
            DisplayMenu(thesocket, state);
        }
    }

    // remove the user that just logged off ..
    for (long unsigned int i = 0; i < myusers.size(); i++)
    {
        if (myusers.at(i).getsocketnumber() == thesocket)
        {
            myusers.erase(myusers.begin() + i);
        }
    }

    cout << "Client Disconnected " << endl;
    close(thesocket);
}

// Display Menu function called by threads .. takes in temp which is the socket and choice ..
void ServerHelper::DisplayMenu(int temp, int choice)
{
    switch (choice)
    {
    case 0: // newline message
        sendresponse(newlinemessage, 1, temp);
        break;
    case 1:
        // send(temp, "Connected , Press enter to continue\n", 38, 0);
        sendresponse(connectedmenu, 38, temp);
        break;
    case 2:
        sendresponse(loginmenu, 68, temp);
        // send(temp, loginmenu, 82, 0);
        break;
    case 3:
        // this will have to be the next menu.
        sendresponse(loggedinmenu, 307, temp);
        break;
    default:
        break;
    }
}

int ServerHelper::statechange(char *message, int mysocket)
{
    char statechangereceivedusername[56];
    char statechangereceivedpassword[56];
    string username, password;
    bool decision;

    stringstream str;
    str << message;
    int choice;
    str >> choice; // convert the message to an int with stringstreams .
    switch (choice)
    {
    case 1:                                     // Login Choice
        send(mysocket, "Username \n", 12, 0);   // prompt username
        bzero(statechangereceivedusername, 56); // receive message
        if (read(mysocket, statechangereceivedusername, 56) < 0)
        {
            perror("Error occured in reading");
        }
        username = statechangereceivedusername;
        //cout << "User name Received " << username << endl;

        send(mysocket, "Password \n", 12, 0);   // prompt password
        bzero(statechangereceivedpassword, 56); // receive message
        if (read(mysocket, statechangereceivedpassword, 56) < 0)
        {
            perror("Error occured in reading");
        }
        password = statechangereceivedpassword;
       // cout << "Password Received" << password << endl;

        decision = usernamepasswordcheck(username, password); // returns true if the username was indeed present

        if (decision)
        { // this means it was  present and password was matched too
            // here we must push to the vector
            if (!username.empty())
            {
                username.pop_back();
            }
            User newuser(username, mysocket);
            myusers.push_back(newuser);
            //cout << "socket number is " << myusers.at(0).getsocketnumber() << endl;
            return loggedinstate;
        }
        else
        {// return wrong password 
            char error[58];
            bzero(error, 58);
            strcpy(error, "Incorrect Password or Username , Press Enter to Continue\n");
            sendresponse(error, 58, mysocket);
        }
        break;
    case 2:                                   // Register Choice
        send(mysocket, "Username \n", 12, 0); // prompt username

        bzero(statechangereceivedusername, 56); // receive message
        if (read(mysocket, statechangereceivedusername, 56) < 0)
        {
            perror("Error occured in reading");
        }
        username = statechangereceivedusername;

        send(mysocket, "Password \n", 12, 0); // prompt password

        bzero(statechangereceivedpassword, 56); // receive message
        if (read(mysocket, statechangereceivedpassword, 56) < 0)
        {
            perror("Error occured in reading");
        }
        password = statechangereceivedpassword;
        decision = usernamecheck(username); // returns true if the username was indeed present

        if (!decision)
        { // given the username was not present .. registration is successsful
            // add it to the file ..
            // open file  WRITING HERE
            string filename("users.txt");
            ofstream file_out;
            strcat(statechangereceivedusername, statechangereceivedpassword);
            file_out.open(filename, ios_base::app);
            for (long unsigned int i = 0; i < strlen(statechangereceivedusername) - 1; i++)
            {
                if (statechangereceivedusername[i] == '\n')
                    statechangereceivedusername[i] = ' ';
            }

            file_out << statechangereceivedusername; // write this to file ..
            file_out.close();
        }
        else
        { // username already present .. return not logged in state
            char error[60];
            bzero(error, 60);
            strcpy(error, "Username is already in the System , Press Enter to Contiue\n");
            sendresponse(error, 60, mysocket);
        }
        break;
    default:
        break;
    }
    return registerstate;
}

bool ServerHelper::usernamecheck(string username)
{
    if (!username.empty()) // delete the endline character
        username.pop_back();
    // READING FILE
    bool usernametoggle = false;
    vector<string> lines;
    string line;
    ifstream input_file("users.txt");
    if (!input_file.is_open()) // see if file is open
    {
        cerr << "Could not open the file - '"
             << "users.txt "
             << "'" << endl;
        return EXIT_FAILURE;
    }

    while (getline(input_file, line)) // get all the lines in file
    {
        lines.push_back(line);
    }

    for (const auto &i : lines) // go through each line and look at the username ..
    {
        stringstream s(i);
        string word;
        while (s >> word)
        {

            if (word == username)
            { // iff the username is present .. return true
                usernametoggle = true;
            }
        }
    }
    return usernametoggle;
}
bool ServerHelper::usernamepasswordcheck(string username, string password)
{
    if (!username.empty()) // delete the endline character
        username.pop_back();
    if (!password.empty()) // delete the endline character
        password.pop_back();
    // READING FILE
    bool usernametoggle = false;
    bool passwordtoggle = false;

    vector<string> lines;
    string line;
    ifstream input_file("users.txt");
    if (!input_file.is_open()) // see if file is open
    {
        cerr << "Could not open the file - '"
             << "users.txt "
             << "'" << endl;
        return EXIT_FAILURE;
    }

    while (getline(input_file, line)) // get all the lines in file
    {
        lines.push_back(line);
    }

    for (const auto &i : lines) // go through each line and look at the username ..
    {
        stringstream s(i);
        string word;
        while (s >> word)
        {

            if (word == username)
            { // iff the username is present .. return true
                usernametoggle = true;
            }
            if (word == password)
            {
                passwordtoggle = true;
            }
        }
    }
    if (usernametoggle && passwordtoggle)
        return true;
    else
        return false;
}
void ServerHelper::loggedinstatechange(char *message, int mysocket)
{
    vector<string> subscriptions;
    vector<string> locations;
    char statechangereceivedpassword[56];
    char statechangereceivedusername[56];
    char statechangenewpassword[56];
    char whichuser[20];
    char location[20];
    char custommessage[custommessagesize];

    bzero(statechangereceivedpassword, 56);  //clear them ? most likely unnecessary actually 
    bzero(statechangereceivedusername, 56);  
    bzero(statechangenewpassword, 56);       
    bzero(whichuser, 20);                    
    bzero(location, 20);                    
    bzero(custommessage, custommessagesize); 

    string username, password, result, newpassword, assembler, temp, usertogetsubs, locationstring;
    bool decision;
    int socketforprivate;

    stringstream str;
    str << message;
    int choice;
    str >> choice;
    switch (choice)
    {
    case 1: // Subscribe to a location
        //****
        send(mysocket, "Username \n", 12, 0);   // prompt username
        bzero(statechangereceivedusername, 56); // receive message
        if (read(mysocket, statechangereceivedusername, 56) < 0)
        {
            perror("Error occured in reading");
        }
        username = statechangereceivedusername;
        if (!username.empty())
        {
            username.pop_back();
        }
        send(mysocket, "What location do you want to subscribe to  \n", 45, 0); // prompt username
        if (read(mysocket, location, 20) < 0)
        {
            perror("Error occured in reading");
        }
        locationstring = location;

        for (long unsigned int i = 0; i < myusers.size(); i++)
        {
            if (myusers.at(i).getusername() == username)
            {
                myusers.at(i).appendlocation(locationstring);
            }
        }
        //***
        break;
    case 2:                                     // unsubscribe to a location
        send(mysocket, "Username \n", 12, 0);   // prompt username
        bzero(statechangereceivedusername, 56); // receive message
        if (read(mysocket, statechangereceivedusername, 56) < 0)
        {
            perror("Error occured in reading");
        }
        username = statechangereceivedusername;
        if (!username.empty())
        {
            username.pop_back();
        }
        //*************
        send(mysocket, "What location do you want to Unsubscribe to  \n", 47, 0); // prompt username

        bzero(custommessage, custommessagesize);
        assembler = "What location do you want to Unsubscribe to  \n\n";
        temp.clear();
        for (long unsigned int i = 0; i < myusers.size(); i++)
        {
            if (myusers.at(i).getusername() == username)
            {
                locations = myusers.at(i).getlocations();
            }
        }
        for (long unsigned int i = 0; i < locations.size(); i++)
        {
            assembler += locations.at(i);
            assembler += "\n";
        }
        assembler += "\n";

        if (assembler.size() < custommessagesize + 1)
        {
            for (long unsigned int i = 0; i < assembler.size(); i++)
            {
                custommessage[i] = assembler.at(i);
            }
        }

        sendresponse(custommessage, assembler.size(), mysocket); // SEND THE ONLINE USERS
        bzero(custommessage, custommessagesize);                 // CLEAR CUSTOM MESSAGE
        assembler.clear();                                       // CLEAR ASSEMBLER
        assembler = "\n";

        if (read(mysocket, location, 20) < 0)
        {
            perror("Error occured in reading");
        }
        locationstring = location;

        for (long unsigned int i = 0; i < myusers.size(); i++)
        {
            if (myusers.at(i).getusername() == username)
            {
                myusers.at(i).removelocation(locationstring);
            }
        }
        break;
    case 3: //SEND A MESSAGE TO A LOCATION

        break;
    case 4: //Send a private message
  // display all online users 
        bzero(custommessage, custommessagesize);
        assembler = "Which User would you like to message?? \n ONLINE USERS BY USERNAME : \n";
        temp.clear();
        for (long unsigned int i = 0; i < myusers.size(); i++)
        {
            temp = myusers.at(i).getusername();
        
            assembler += temp;
            assembler += " \n";
            temp.clear();
        }
        if (assembler.size() < custommessagesize + 1)
        {
            for (long unsigned int i = 0; i < assembler.size(); i++)
            {
                custommessage[i] = assembler.at(i);
            }
        }

        sendresponse(custommessage, assembler.size(), mysocket); // SEND THE ONLINE USERS
        bzero(custommessage, custommessagesize);                 // CLEAR CUSTOM MESSAGE
        assembler.clear();
        assembler = "\n";
        if (read(mysocket, whichuser, 56) < 0)
        {
            perror("Error occured in reading");
        }
        usertogetsubs = whichuser; // RECEIVED USERNAME THAT WE WANT TO KNOW SUBSCRIPTIONS OF
        if (!usertogetsubs.empty())
        {
            usertogetsubs.pop_back();
        }

         for (long unsigned int i = 0; i < myusers.size(); i++)
        {
            temp = myusers.at(i).getusername();
        
           if(myusers.at(i).getusername() == usertogetsubs)
           {
               socketforprivate = myusers.at(i).getsocketnumber();
           }
        }

        send(mysocket, "What shall be the message \n", 30, 0);   // prompt message
        bzero(statechangereceivedusername, 56); // receive message
        username.clear();
        if (read(mysocket, statechangereceivedusername, 56) < 0)
        {
            perror("Error occured in reading");
        }
        username = statechangereceivedusername;
        last10messages.push_back(username);
        sendresponse(statechangereceivedusername,strlen(statechangereceivedusername) , socketforprivate);

        break;
    case 5: //See all the locations you are subscribed to ... I dont think im implimenting this option .. maybe .. 
         bzero(custommessage, custommessagesize);
        assembler = "ONLINE USERS BY USERNAME : \n";
        temp.clear();
        for (long unsigned int i = 0; i < myusers.size(); i++)
        {
            temp = myusers.at(i).getusername();
        
            assembler += temp;
            assembler += " \n";
            temp.clear();
        }
        if (assembler.size() < custommessagesize + 1)
        {
            for (long unsigned int i = 0; i < assembler.size(); i++)
            {
                custommessage[i] = assembler.at(i);
            }
        }

        sendresponse(custommessage, assembler.size(), mysocket); // SEND THE ONLINE USERS
        bzero(custommessage, custommessagesize);                 // CLEAR CUSTOM MESSAGE
        assembler.clear();                                       // CLEAR ASSEMBLER
        assembler = "\n";
        if (read(mysocket, whichuser, 56) < 0)
        {
            perror("Error occured in reading");
        }
        usertogetsubs = whichuser; // RECEIVED USERNAME THAT WE WANT TO KNOW SUBSCRIPTIONS OF
        if (!usertogetsubs.empty())
        {
            usertogetsubs.pop_back();
        }
        for (long unsigned int i = 0; i < myusers.size(); i++)
        {
            if (myusers.at(i).getusername() == usertogetsubs)
            {
                subscriptions = myusers.at(i).getlocations();
            }
        } // ASSIGNED THE RIGHT VECTOR OF LOCATIONS NOW

        for (long unsigned int i = 0; i < subscriptions.size(); i++)
        {
            assembler += subscriptions.at(i);
        } // APPEND THAT TO THE ASSEMBLER STRING
        for (; subscriptions.size() != 0;)
        {
            subscriptions.erase(subscriptions.begin());
        } // CLEAR THE SUBSCRIPTIONS VECTOR AS IT IS NOT NEEDED ANYMORE

        if (assembler.size() < custommessagesize + 1)
        {
            for (long unsigned int i = 0; i < assembler.size(); i++)
            {
                custommessage[i] = assembler.at(i);
            }
        }                                                        // ASSIGN THE MESSAGE AGAIN
        sendresponse(custommessage, assembler.size(), mysocket); // SEND THE ONLINE USERS
    break;
    case 6: // see all online users
      bzero(custommessage, custommessagesize);
        assembler = "ONLINE USERS BY USERNAME : \n";
        temp.clear();
        for (long unsigned int i = 0; i < myusers.size(); i++)
        {
            temp = myusers.at(i).getusername();
        
            assembler += temp;
            assembler += " \n";
            temp.clear();
        }
        if (assembler.size() < custommessagesize + 1)
        {
            for (long unsigned int i = 0; i < assembler.size(); i++)
            {
                custommessage[i] = assembler.at(i);
            }
        }

        sendresponse(custommessage, assembler.size(), mysocket); // SEND THE ONLINE USERS
        bzero(custommessage, custommessagesize);                 // CLEAR CUSTOM MESSAGE
        assembler.clear();                                       // CLEAR ASSEMBLER

    break;
    case 7: // see last 10 messages 
        bzero(custommessage, custommessagesize);
        assembler = "Last 10 messages that were sent : \n";
        temp.clear();
        for (long unsigned int i = 0; i < last10messages.size(); i++)
        {
            temp = last10messages.at(i);
        
            assembler += temp;
            assembler += " \n";
            temp.clear();
        }
        if (assembler.size() < custommessagesize + 1)
        {
            for (long unsigned int i = 0; i < assembler.size(); i++)
            {
                custommessage[i] = assembler.at(i);
            }
        }

        sendresponse(custommessage, assembler.size(), mysocket); // SEND THE ONLINE USERS
    break;
    case 8: // change password 
     //**
        send(mysocket, "Username \n", 12, 0);   // prompt username
        bzero(statechangereceivedusername, 56); // receive message
        if (read(mysocket, statechangereceivedusername, 56) < 0)
        {
            perror("Error occured in reading");
        }
        username = statechangereceivedusername;
        send(mysocket, "Password \n", 12, 0);   // prompt password
        bzero(statechangereceivedpassword, 56); // receive message
        if (read(mysocket, statechangereceivedpassword, 56) < 0)
        {
            perror("Error occured in reading");
        }
        password = statechangereceivedpassword;
        decision = usernamepasswordcheck(username, password); // returns true if the username was indeed present

        if (decision)
        {
            // prompt the new password
            send(mysocket, "New Password \n", 16, 0); // prompt password
            if (read(mysocket, statechangenewpassword, 56) < 0)
            {
                perror("Error occured in reading");
            }
            newpassword = statechangenewpassword;
            /*****************/
            // Reading the file
            string myline;
            ifstream input__file("users.txt");
            if (!input__file.is_open())
            {
                cerr << "Could not open the file - '"
                     << "users.txt"
                     << "'" << endl;
                break;
            }

            ofstream temp;
            temp.open("temp.txt");
            string mycopy;
            if (!username.empty())
                username.pop_back();

            while (getline(input__file, myline))
            {

                if (myline.substr(0, username.size()).c_str() != username)
                {
    
                    temp << myline << endl;
                }
              
            }
            input__file.close();
            temp.close();
            remove("users.txt");
            rename("temp.txt", "users.txt");
            // at this point we deleted the line

            string filename("users.txt");
            ofstream file_out;
            file_out.open(filename, ios_base::app);
            strcat(statechangereceivedusername, statechangenewpassword);

            for (long unsigned int i = 0; i < strlen(statechangereceivedusername) - 1; i++)
            {
                if (statechangereceivedusername[i] == '\n')
                    statechangereceivedusername[i] = ' ';
            }

            file_out << statechangereceivedusername; // write this to file ..
            file_out.close();
        }
    break;

    default:
        break;
    }
}
