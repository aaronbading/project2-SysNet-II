#include "serverhelper.h"
#include <string.h>
ServerHelper::ServerHelper()
{
    bzero(loginmenu, 68);
    strcpy(loginmenu, "\n\n Press 1 to Login \n Press 2 to Register \n Type 'quit' to Quit ");
    bzero(connectedmenu, 38);
    strcpy(connectedmenu, "Connected , Press enter to continue\n");
    bzero(newlinemessage, 1);
    strcpy(newlinemessage, "\n");
    bzero(loggedinmenu, 174);
    strcpy(loggedinmenu, "\nSuccessfully logged in \n 1. Subscribe to a location \n 2. Unsubscribe from a location \n 3. Change Password \n 4. See all subscriptions for a user \n Type quit to Exit\n\n");
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
        mythread.detach();
    }
    close(server_filedescriptor);
}

void ServerHelper::sendresponse(void *message, int msglen, int created_socket)
{
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
{ // thread function
    char receivedmessage[56];
    bzero(receivedmessage, 56);
    int state = 2;
    int thesocket = mytempsocket;
    DisplayMenu(thesocket, 1);
    DisplayMenu(thesocket, 2);

    while (strcmp(receivedmessage, "quit\n"))
    { // while exit hasnt been typed in
        bzero(receivedmessage, 56);
        if (read(thesocket, receivedmessage, 56) < 0)
        {
            perror("Error occured in reading");
        }

        // send(thesocket, receivedmessage, strlen(receivedmessage)+1, 0); // echo the message
        cout << "Received message from socket :  " << thesocket << " is " << receivedmessage << endl;
        // send(thesocket, mymessage, strlen(mymessage) + 1, 0);

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
        sendresponse(loggedinmenu, 174, temp);
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
    // convert the message to an int.
    stringstream str;
    str << message;
    int choice;
    str >> choice;
    switch (choice)
    {
    case 1: // Login Choice
        cout << " choice is 1 " << endl;
        send(mysocket, "Username \n", 12, 0); // prompt username

        bzero(statechangereceivedusername, 56); // receive message
        if (read(mysocket, statechangereceivedusername, 56) < 0)
        {
            perror("Error occured in reading");
        }
        username = statechangereceivedusername;
        cout << "User name Received " << username << endl;

        send(mysocket, "Password \n", 12, 0);   // prompt password
        bzero(statechangereceivedpassword, 56); // receive message
        if (read(mysocket, statechangereceivedpassword, 56) < 0)
        {
            perror("Error occured in reading");
        }
        password = statechangereceivedpassword;
        cout << "Password Received" << password << endl;

        // TODO .. check if this is already present ..
        decision = usernamepasswordcheck(username, password); // returns true if the username was indeed present

        if (decision)
        { // this means it was  present and password was matched too
            return loggedinstate;
        }
        return registerstate;
        break;
    case 2: // Register Choice
        cout << " choice is 2 " << endl;
        send(mysocket, "Username \n", 12, 0); // prompt username

        bzero(statechangereceivedusername, 56); // receive message
        if (read(mysocket, statechangereceivedusername, 56) < 0)
        {
            perror("Error occured in reading");
        }
        username = statechangereceivedusername;
        cout << "User name Received " << username << endl;

        send(mysocket, "Password \n", 12, 0); // prompt password

        bzero(statechangereceivedpassword, 56); // receive message
        if (read(mysocket, statechangereceivedpassword, 56) < 0)
        {
            perror("Error occured in reading");
        }
        password = statechangereceivedpassword;
        cout << "Password Received " << password << endl;

        // TODO .. Check if username is already present or not .. if not then add it to the file of users...

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
            cout << " returns logged in " << endl;
        }
        else
        { // username already present .. return not logged in state
            cout << " returns register state " << endl;
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
bool ServerHelper::passwordcheck(string password)
{

    if (!password.empty()) // delete the endline character
        password.pop_back();
    // READING FILE
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

            if (word == password)
            {
                passwordtoggle = true;
            }
        }
    }
    return passwordtoggle;
}
void ServerHelper::loggedinstatechange(char *message, int mysocket)
{
    char statechangereceivedpassword[56];
    char statechangereceivedusername[56];
    char statechangenewpassword[56];

    bzero(statechangereceivedpassword, 56); // receive message
    bzero(statechangereceivedusername, 56); // receive message
    bzero(statechangenewpassword, 56);      // receive message

    string username, password, result, newpassword;
    bool decision;

    stringstream str;
    str << message;
    int choice;
    str >> choice;
    switch (choice)
    {
    case 0: // Quit
        break;
    case 1: // Subscribe to a location
        cout << "subscribe " << endl;
        break;
    case 2: // unsubscribe to a location
        cout << "unsubscribe " << endl;

        break;
    case 3: // Change Password
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
    case 4: // See all subscriptions for a given user .
        cout << " see subscriptions " << endl;

        break;
    default:
        break;
    }
}
