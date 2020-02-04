#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <winsock2.h>
#include "cJSON.c"
#include <string.h>
#include <windows.h>
#include <conio.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr

void sendreq( char req[] , char reqtype[] , char reqtoken[] );
void account_menu ();
void main_menu();
void chat_menu();


char reqtoken[2000];
char username[100];
char password[100];
char req[1000];
char reqtype[100];
char channel_name[100];
char sendreqtoken[1000];
char message[1000];
int set_color;

/*void chat(int server_socket)
{
	char buffer[MAX];
	int n;
	while (true) {
		memset(buffer, 0, sizeof(buffer));
		printf("Enter your message: ");
		n = 0;

		// Copy client message to the buffer
		while ((buffer[n++] = getchar()) != '\n')
			;

		// Send the buffer to server
		send(server_socket, buffer, sizeof(buffer), 0);
		memset(buffer, 0, sizeof(buffer));

		// Read the message from server and copy it to buffer
		recv(server_socket, buffer, sizeof(buffer), 0);

		// Print buffer which contains the server message
		printf("From server: %s", buffer);

		// If the message starts with "exit" then client exits and chat ends
		if ((strncmp(buffer, "exit", 4)) == 0) {
			printf("Client stopping...\n");
			return;
		}
	}
}*/

void SetColor(int ForgC)
{
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

int arrow(int max)
{
    int arw;
    int what_to_do=0;
    while (1)
    {
        arw=getch();
        if (arw==80 && what_to_do<max)
        {
            what_to_do++;
            //printf("Down ");
        }
        if (arw==72 && what_to_do>1)
        {
            what_to_do--;
            //printf("Up ");
        }
        if (max==2)
        {
            system("cls");
            if (what_to_do==1)
            {
                printf("Account Menu\n");
                SetColor(4);
                printf("1: Register\n");
                SetColor(3);
                printf("2: Login\n");
            }
            if (what_to_do==2)
            {
                printf("Account Menu\n");
                printf("1: Register\n");
                SetColor(4);
                printf("2: Login\n");
                SetColor(3);
            }
        }
        if (max==3)
        {
            system("cls");
            if (what_to_do==1)
            {
                SetColor(4);
                printf("1: Create Channel\n");
                SetColor(3);
                printf("2: Join Channel\n");
                printf("3: Logout\n");
            }
            if (what_to_do==2)
            {
                printf("1: Create Channel\n");
                SetColor(4);
                printf("2: Join Channel\n");
                SetColor(3);
                printf("3: Logout\n");
            }
            if (what_to_do==3)
            {
                printf("1: Create Channel\n");
                printf("2: Join Channel\n");
                SetColor(4);
                printf("3: Logout\n");
                SetColor(3);
            }
        }
        if (max==4)
        {
            system("cls");
            if (what_to_do==1)
            {
                SetColor(4);
                printf("1: Send Message\n");
                SetColor(3);
                printf("2: Refresh\n");
                printf("3: Channel Members\n");
                printf("4: Leave Channel\n");
            }
            if (what_to_do==2)
            {
                printf("1: Send Message\n");
                SetColor(4);
                printf("2: Refresh\n");
                SetColor(3);
                printf("3: Channel Members\n");
                printf("4: Leave Channel\n");
            }
            if (what_to_do==3)
            {
                printf("1: Send Message\n");
                printf("2: Refresh\n");
                SetColor(4);
                printf("3: Channel Members\n");
                SetColor(3);
                printf("4: Leave Channel\n");
            }
            if (what_to_do==4)
            {
                printf("1: Send Message\n");
                printf("2: Refresh\n");
                printf("3: Channel Members\n");
                SetColor(4);
                printf("4: Leave Channel\n");
                SetColor(3);
            }
        }
        if (arw==13) break;
    }
    printf("\n");
    return what_to_do;
}

int sock_create()
{
	int client_socket, server_socket;
	struct sockaddr_in servaddr, cli;

	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

	// Create and verify socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");

	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// Connect the client socket to server socket
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
	else{
		printf("Successfully connected to the server..\n");
        return client_socket;
	// Function for chat
    }

	// Close the socket
	return 0;
}


int main()
{
    account_menu ();
}

void sendreq( char req[] , char reqtype[] , char reqtoken[] )
{
    int socket;
    char req2[1000];
    strcpy(req2 , req); // this char* used to determine channel member request and refresh request
    socket = sock_create();
    send(socket , req , 1000 , 0);
    memset(req , 0 , sizeof(req));
    recv(socket , req , 1000 , 0);
    cJSON* jsonrec = cJSON_Parse(req);
    cJSON* jsontype = cJSON_GetObjectItem(jsonrec , "type");
    cJSON* jsoncontent = cJSON_GetObjectItem(jsonrec, "content");
    strcpy(reqtype , jsontype -> valuestring);
    if (reqtype[0]!='L') // cheking that request type is list or not
    {
        strcpy(reqtoken , jsoncontent -> valuestring);
    }
    int size = cJSON_GetArraySize(jsoncontent);
    if (reqtype[0]=='L')
    {
        if (req2[0]=='c')
        {
            for (int i=0 ; i<size ; i++)
            {
                cJSON* test = cJSON_GetArrayItem(jsoncontent,i);
                printf ("%s\n", test->valuestring);
            }
        }
        else
        {
            for (int i=0 ; i<size ; i++)
            {
                cJSON *listget = cJSON_GetArrayItem(jsoncontent,i);
                cJSON* listsender = cJSON_GetObjectItem(listget,"sender");
                cJSON* listcontent = cJSON_GetObjectItem(listget,"content");
                printf("%s: %s\n", listsender -> valuestring , listcontent -> valuestring );
            }
        }
    }
}

void account_menu ()
{
    system("COLOR F3");
    memset(req , 0 , sizeof(req));
    printf("Account Menu\n");
    printf("1: Register\n");
    printf("2: Login\n");
    int WhatToDo;
    //scanf("%d", &WhatToDo);
    WhatToDo=arrow(2);
    if (WhatToDo==1)
    {
        getchar();
        printf("Please Enter Your Username\n");
        scanf("%[^\n]s", username);
        getchar();
        printf("Please Enter Your Password\n");
        scanf("%[^\n]s", password);
        strcat(req , "register ");
        strcat(req , username);
        strcat(req , ", ");
        strcat(req , password);
        strcat(req , "\n");
        sendreq(req , reqtype , reqtoken);
        memset(reqtoken , 0 , sizeof(reqtoken));
        if (reqtype[0]=='E')
        {
            printf("This userename has been already taken, Please try again\n");
        }
        //system("cls");
         return account_menu ();
    }
    else
    {
        getchar();
        printf("Please Enter Your Username\n");
        scanf("%[^\n]s", username);
        printf("Please Enter Your Password\n");
        getchar();
        scanf("%[^\n]s", password);
        strcat(req , "login ");
        strcat(req , username);
        strcat(req , ", ");
        strcat(req , password);
        strcat(req , "\n");
        sendreq(req , reqtype , reqtoken);
        memset(sendreqtoken , 0 , sizeof(sendreqtoken));
        int i=0;
        while(reqtoken[i]!='\0')
        {
            sendreqtoken[i] = reqtoken[i]; // here we save token that server sent in sendreqtoken
            i++;
        }
        if (reqtype[0]=='E') //this error appears when the username does not exist
        {
            printf("%s\n",reqtoken);
            account_menu ();
        }
        else return main_menu();
        //system("cls");
    }
}

void main_menu()
{
    memset(req , 0 , sizeof(req));
    memset(reqtoken , 0 , sizeof(reqtoken));
    //set a theme for chat application
    //printf("Please select a color theme\n");
    //scanf("%d", &set_color);
    //SetColor(set_color);
    //system("cls");
    printf("1: Create Channel\n");
    printf("2: Join Channel\n");
    printf("3: Logout\n");
    int WhatToDo;
    //scanf("%d", &WhatToDo);
    WhatToDo=arrow(3);
    int i=0;
    while(sendreqtoken[i]!='\0')
    {
        reqtoken[i] = sendreqtoken[i];
        i++;
    }
    if (WhatToDo==1)
    {
        printf("Please enter your channel's name\n");
        getchar();
        scanf("%[^\n]s", channel_name);
        strcat(req , "create channel ");
        strcat(req , channel_name);
        strcat(req , ", ");
        strcat(req , reqtoken);
        strcat(req , "\n");
        //printf("%s\n",reqtoken);
        sendreq(req , reqtype , reqtoken);
        if (reqtype[0]=='S')
        {
            printf("Channel has been successfuly created\n");
            chat_menu();
        }
        else
        {
            printf("This channel's name has been already taken, Please try again\n");
            main_menu();
        }
        //system("cls");
    }
    if (WhatToDo==2)
    {
        printf("Please enter channel's name\n");
        getchar();
        scanf("%[^\n]s", channel_name);
        strcat(req , "join channel ");
        strcat(req , channel_name);
        strcat(req , ", ");
        strcat(req , reqtoken);
        strcat(req , "\n");
        sendreq(req , reqtype , reqtoken);
        if (reqtype[0]=='S')
        {
            printf("You joined the channel successfuly\n");
            chat_menu();
        }
        else
        {
            printf("This channel's name does not exist, Please try again\n");
            main_menu();
        }
        //system("cls");
    }
    if (WhatToDo==3)
    {
        strcat(req , "logout ");
        strcat(req , reqtoken);
        strcat(req , "\n");
        sendreq(req , reqtype , reqtoken);
        //system("cls");
        account_menu ();
    }
}

void chat_menu()
{
    memset(req , 0 , sizeof(req));
    memset(reqtoken , 0 , sizeof(reqtoken));
    printf("1: Send Message\n");
    printf("2: Refresh\n");
    printf("3: Channel Members\n");
    printf("4: Leave Channel\n");
    int WhatToDo;
    //scanf("%d", &WhatToDo);
    WhatToDo=arrow(4);
    int i=0;
    while(sendreqtoken[i]!='\0')
    {
        reqtoken[i] = sendreqtoken[i];
        i++;
    }
    if (WhatToDo==1)
    {
        memset(message , 0 , sizeof(message));
        getchar();
        scanf("%[^\n]s", message);
        strcat(req, "send ");
        strcat(req, message);
        strcat(req, ", ");
        strcat(req, reqtoken);
        strcat(req, "\n");
        sendreq(req , reqtype , reqtoken);
        chat_menu();
    }

    if (WhatToDo==2)
    {
        strcat(req, "refresh ");
        strcat(req, reqtoken);
        strcat(req, "\n");
        sendreq(req , reqtype , reqtoken);
       // printf("%s\n", reqtoken);
        chat_menu();
    }

    if (WhatToDo==3)
    {
        strcat(req, "channel members ");
        strcat(req, reqtoken);
        strcat(req, "\n");
        sendreq(req , reqtype , reqtoken);
        //printf("%s\n", reqtoken);
        chat_menu();
    }

    if (WhatToDo==4)
    {
        strcat(req, "leave ");
        strcat(req, reqtoken);
        strcat(req, "\n");
        sendreq(req , reqtype , reqtoken);
        main_menu();
    }
}
