# project2-SysNet-II
TCP Weather station with communcation
This is a two in one c++ program that includes a multithreaded server and a client that is able to log into the server. The server is able to handle multiple clients at once and is able to handle their requests simultaneously with multiple threads

## How to use
After compiling the program inside of the uwf ssh server by entering the command "make", you can run server by typing  **"./server".** in the command line . After this , one must open multiple other windows and go to the same directory and type **"./client".**. 

## Results

The result of this will be that the server will accept incoming clients and their commands. 
The initial menu will give you the option to either register or login .
Once successfully registered, the user is now able to **"subscribe to a location"** , **"unsubscribe from a location"** ,**" change the password"** and **"view all the subscriptions from a given user"**.
The client can quit at any time by typing quit as a command.
The server will be shut down by pressing **"CTRL-V"**


## Conclusion
After all , this project aided in understanding multi-thread programming both in c and c++ and shed light on how powerful it can be. In addition, this project was able to show how one can underestimate the difficulty of a seemingly easy project.  