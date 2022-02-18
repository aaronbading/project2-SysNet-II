#include "helper.h"
#include <string.h>
#include <netdb.h>

Helper::Helper()
{
    readfiles();
    //*************
    // this is mainly setup code
    // Creating socket file descriptor is really creating a tcp socket

    // For TCP/IP sockets, we want to specify the :  IP address family (AF_INET)
    //     virtual circuit service (SOCK_STREAM).
    //     Since there’s only one form of virtual circuit service,
    //     there are no variations of the protocol, so the last argument, protocol, is zero.

    // virtual circuit service  is sock_stream
    if ((server_filedescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error occured in socket");
        exit(EXIT_FAILURE);
    }

    mysocketaddress.sin_family = AF_INET;         // the IP address family
    mysocketaddress.sin_addr.s_addr = INADDR_ANY; // the address for the socket which is just an ip
    mysocketaddress.sin_port = htons(PORT);       // the transport address which is the port number

    memset(mysocketaddress.sin_zero, '\0', sizeof(mysocketaddress.sin_zero));

    // bind process parameters are the socket , socketaddress and the size of socketaddress
    if (bind(server_filedescriptor, (struct sockaddr *)&mysocketaddress, sizeof(mysocketaddress)) < 0)
    {
        perror("Error occured in bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_filedescriptor, 10) < 0)
    {
        perror("Error occured in listen");
        exit(EXIT_FAILURE);
    }
    //*************************************
    start();
}

void Helper::start()
{
    while (1)
    {
        printf("\n********** Waiting for new a Request **********\n\n");
        if ((created_socket = accept(server_filedescriptor, (struct sockaddr *)&mysocketaddress, (socklen_t *)&addresslength)) < 0)
        {
            perror("Error occured in accept");
            exit(EXIT_FAILURE);
        }
        char buffer[30000];
        readvalue = read(created_socket, buffer, 30000);
        if(fork()==0)
        {
            printf("%s\n",buffer);
            interpretRequest(buffer);        
            printf("********** Message was sent **********");
            exit(EXIT_SUCCESS);
            printf("ThIS STATEMENT SHOULD NOT PRINT ");
        }

        close(created_socket);
    }
}

void Helper::interpretRequest(char *input)
{
    char type[10];  // the type of content
    char imagename[17]; // name of image
    bool catchit = false;   // this will see if we have the file 
    char *pch, *ich;
    pch = strtok(input, "\n");

    int i = 0;
    ich = strtok(pch, " "); // the delimeter is now an empty space
    while (ich != NULL)
    {
        if (i == 1)
        {
            if (!strcmp(ich, "/"))
            { // default page
                strcpy(type, "text/html");
                createMessage(defaultfile, type);
                catchit = true;
            }
            if (!strcmp(ich, "/index.html"))
            { // index.html
                strcpy(type, "text/html");
                createMessage(defaultfile, type);
                catchit = true;
            }
            if (!strcmp(ich, "/something.html"))
            { // index.html
                strcpy(type, "text/html");
                createMessage(indexfile, type);
                catchit = true;
            }
            if (!strcmp(ich, "/testpresence.html"))
            {
                // send
                strcpy(imagename, "./codephoto.jpeg");
                sendpicture(imagename);
                strcpy(type, "text/html");
                createMessage(picturefile, type);
                catchit = true;
            }
            if (!catchit)
            { // if catchit remained false that means we attained an unknow file name and therefore we must somehow send the code 404
                strcpy(type, "text/html");
                createMessage(notfoundfile, type);
            }
        }

        ich = strtok(NULL, " ");
        i++;
    }
}

void Helper::readfiles()
{

    char file1name[25], file2name[25], file3name[25], file4name[25];
    char *files[4];
    strcpy(file1name, "defaultpage.html");
    strcpy(file2name, "something.html");
    strcpy(file3name, "notfound.html");
    strcpy(file4name, "testpresence.html");
    files[0] = file1name;
    files[1] = file2name;
    files[2] = file3name;
    files[3] = file4name;

    int filesize = 1500;
    char myfile[filesize];
    char line[80] = {0};
    FILE *inputfp1;
    for (int i = 0; i < 4; i++)
    {
        inputfp1 = fopen(files[i], "r"); // Open file for read.
        if (inputfp1 == NULL)
        {
            printf("Error opening file");
            exit(0);
        }

        char lastline[8];
        strcpy(lastline, "</html>");
        int totalcount = 0;
        while (fgets(line, 80, inputfp1) != NULL)
        {
            if (strcmp(line, "</html>"))
            {

                for (size_t i = 0; i < strlen(line); i++)
                {
                    myfile[totalcount] = line[i];
                    totalcount++;
                }
            }
            else
            {
                for (size_t i = 0; i < strlen(lastline); i++)
                {
                    myfile[totalcount] = lastline[i];
                    totalcount++;
                }
                break;
            }
        }
        fclose(inputfp1); // close file

        // copy content of myfile ... into the correct file
        for (size_t j = 0; j < strlen(myfile); j++)
        {
            if (i == 0)
                defaultfile[j] = myfile[j];
            if (i == 1)
                indexfile[j] = myfile[j];
            if (i == 2)
                notfoundfile[j] = myfile[j];
            if (i == 3)
                picturefile[j] = myfile[j];
        }
        for (int j = 0; j < filesize; j++)
        {
            myfile[j] = '\0';
        }
    }
}

void Helper::createMessage(char *myfile, char *type)
{
    sprintf(header, "HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n", type, strlen(myfile));
    sendresponse(header, strlen(header)); // send the header with content type and length

    // clear header
    for (int i = 0; i < 240; i++)
    {
        header[i] = '\0';
    }
    sendresponse(myfile, strlen(myfile)); // send the actual file
}

void Helper::sendresponse(void *message, int msglen)
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

void Helper::sendpicture(char *image_path)
{
    struct stat stat_buf; /* hold information about input file */
    char temp = 'a';
    char *imagetype = &temp;
    strcpy(imagetype, "image/jpeg");
    sprintf(header, "HTTP/1.0 200 OK\r\nContent-Type: %s\r\n\r\n", imagetype);
    sendresponse(header, strlen(header));

    int fdimg = open(image_path, O_RDONLY);
    fstat(fdimg, &stat_buf);
    int img_total_size = stat_buf.st_size;
    int block_size = stat_buf.st_blksize;

    while (img_total_size > 0)
    {
        if (img_total_size < block_size)
        {
            sendfile(created_socket, fdimg, NULL, img_total_size);
        }
        else
        {
            sendfile(created_socket, fdimg, NULL, block_size);
        }
        img_total_size = img_total_size - block_size;
    }
    close(fdimg);
}