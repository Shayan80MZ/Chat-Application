#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <winsock2.h>
#include <dir.h>
#include <process.h>
#include "cJSON.c"
#include <string.h>
#include <time.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr

char req[3000];
char* res;
int i=-1;
previous_arraaynum=0;
struct users_struct
{
    char name[100];
    char token[100];
    char channel[100];
}users[1000];
int socket_maker(int server_socket){
    struct sockaddr_in client;
        if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    // Accept the data packet from client and verify
    int len = sizeof(client);
    int client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else{
        printf("Server acccepted the client..\n");

    // Function for chatting between client and server
    return client_socket;
    }
}

void recvreq (int server_socket)
{
    int socket;
    socket = socket_maker(server_socket);
    recv(socket , req , 1000 , 0);
    if (strncmp(req , "register " , 9)==0) register_func();
    if (strncmp(req , "login ", 6)==0) login_func();
    if (strncmp(req , "logout ", 7)==0) logout_func();
    if (strncmp(req , "create ", 7)==0) create_channel_func();
    if (strncmp(req , "join ", 5)==0) join_channel_func();
    if (strncmp(req , "send ", 5)==0) send_message_func();
    if (strncmp(req , "channel ", 8)==0) channel_memebers_func();
    if (strncmp(req , "refresh ", 8)==0) refresh_func();
    if (strncmp(req , "leave ", 6)==0) leave_channel_func();
    printf("%s\n",res); //respond
    send(socket , res , 1000 , 0);
    closesocket(socket);
}

void token_creator(char name[])
{
    srand(time(NULL));
    for (int j=0 ; j<32 ; j++)
    {
        int k;
        k = rand()%3;
        if (k==2) users[i].token[j] = 97 + (rand()%26);
        if (k==1) users[i].token[j] = 65 + (rand()%26);
        if (k==0) users[i].token[j] = 48 + (rand()%10);
    }
}

void register_func()
{
    char name[100];
    char pass[100];
    char htgt[100]; //how to get there in file
    sscanf(req , "%*s%s%s" , name , pass);
    int name_size; //this is for deleting "," in the end of name
    //name[strlen(name)-1]='\0';
    //printf("%s\n",name);
    mkdir("Resources");
    mkdir("Resources/Users");
    mkdir("Resources/Channels");
    strcpy(htgt , "Resources/Users/");
    strcat(htgt , name);
    strcat(htgt , ".json");
    FILE* user;
    if ((user = fopen(htgt, "r"))==NULL)
    {
        user = fopen(htgt, "w");
        cJSON* client = cJSON_CreateObject();
        cJSON_AddStringToObject(client , "username" , name);
        cJSON_AddStringToObject(client , "password" , pass);
        char* jsonclient = cJSON_PrintUnformatted(client);
        fprintf(user , "%s" , jsonclient);
        cJSON* success = cJSON_CreateObject();
        cJSON_AddStringToObject(success , "type" , "Successful");
        cJSON_AddStringToObject(success , "content" , "");
        res = cJSON_PrintUnformatted(success); //respond

        fclose(user);
    }
    else
    {
        cJSON* error = cJSON_CreateObject();
        cJSON_AddStringToObject(error , "type" , "Error");
        cJSON_AddStringToObject(error , "content" , "This username has been alredy taken");
        res = cJSON_PrintUnformatted(error); //respond
    }
}

void login_func()
{
    char name[100];
    char pass[100];
    char pass_check[100];
    char htgt[100]; //how to get there in file
    sscanf(req , "%*s%s%s" , name , pass);
    //mkdir("Resources");
    //mkdir("Resources/Users");
    //mkdir("Resources/Channels");
    strcpy(htgt , "Resources/Users/");
    strcat(htgt , name);
    strcat(htgt , ".json");
    FILE* user;
    if ((user = fopen(htgt, "r"))==NULL)
    {
        cJSON* error = cJSON_CreateObject();
        cJSON_AddStringToObject(error , "type" , "Error");
        cJSON_AddStringToObject(error , "content" , "This username dosen't exist");
        res = cJSON_PrintUnformatted(error);
    }
    else
    {
        fscanf(user , "%s" , pass_check);   //pass_check=correct pass & pass=pass that client sent
        fclose(user);
        cJSON* pass_check_json = cJSON_Parse(pass_check);
        cJSON* json_pass = cJSON_GetObjectItem(pass_check_json , "password");
        memset(pass_check , 0 , sizeof(pass_check));
        strcpy(pass_check , json_pass -> valuestring);
        if (strcmp(pass_check , pass)==0)
        {
            i++;
            token_creator(name);
            //printf("%s\n",users[i].token);
            strcpy(users[i].name , name);
            cJSON* success = cJSON_CreateObject();
            cJSON_AddStringToObject(success , "type" , "AuthToken");
            cJSON_AddStringToObject(success , "content" , users[i].token);
            res = cJSON_PrintUnformatted(success); //respond
        }
        else
        {
            cJSON* error = cJSON_CreateObject();
            cJSON_AddStringToObject(error , "type" , "Error");
            cJSON_AddStringToObject(error , "content" , "Password is not correct");
            res = cJSON_PrintUnformatted(error);
        }
    }
}

void logout_func()
{
    char logout_token[100];
    sscanf(req , "%*s%s",logout_token);
    for (int j=0 ; j<=i ; j++)
    {
        if (strcmp(users[j].token , logout_token)==0)
        {
            memset(users[j].name , 0 , sizeof(users[j].name));
            memset(users[j].token , 0 , sizeof(users[j].token));
            memset(users[j].channel , 0 , sizeof(users[j].channel));
        }
    }
    cJSON* success = cJSON_CreateObject();
    cJSON_AddStringToObject(success , "type" , "Successful");
    cJSON_AddStringToObject(success , "content" , "");
    res = cJSON_PrintUnformatted(success); //respond
}

void create_channel_func()
{
    char token_check[100];
    char channel_name[100];
    char htgt[100]; // file way
    sscanf(req , "%*s%*s%s%s" , channel_name , token_check);
    //printf("%s\n",token_check);
    int index;
    int check=0;
    for (int j=0 ; j<=i ; j++)
    {
        if (strncmp(users[j].token , token_check , 32)==0)
        {
            check=1;
            index=j;
            break;
        }
    }
    if (check==1) // checking that token is correct or not
    {
        strcpy(htgt , "Resources/Channels/");
        strcat(htgt , channel_name);
        strcat(htgt , ".json");
        FILE* user;
        if ((user=fopen(htgt , "r"))==NULL)
        {
            fclose(user);
            user=fopen(htgt , "w");
            cJSON* bracket = cJSON_CreateObject();
            cJSON* inbracket = cJSON_CreateArray();
            cJSON* a;
            cJSON_AddItemToArray(inbracket , a=cJSON_CreateObject());
            char name[100];
            strcpy(name , users[index].name);
            strcat(name , " Created -");
            strcat(name , channel_name);
            strcat(name , "-");
            strcpy(users[index].channel , channel_name);
            cJSON_AddStringToObject(a , "sender" , "Server");
            cJSON_AddStringToObject(a , "content" , name);
            cJSON_AddItemToObject(bracket , "messages" , inbracket);
            char* jsonchannel = cJSON_PrintUnformatted(bracket);
            //printf("%s\n",jsonchannel);
            fprintf(user ,"%s%c" , jsonchannel , '\n');
            cJSON* success=cJSON_CreateObject();
            cJSON_AddStringToObject(success , "type" , "Successful");
            cJSON_AddStringToObject(success , "content" , "");
            res = cJSON_PrintUnformatted(success);
            fclose(user);
        }
        else
        {
            cJSON* error = cJSON_CreateObject();
            cJSON_AddStringToObject(error , "type" , "Error");
            cJSON_AddStringToObject(error , "content" , "This channel name has been already token");
            res = cJSON_PrintUnformatted(error);
        }
    }
    else
    {
            cJSON* error = cJSON_CreateObject();
            cJSON_AddStringToObject(error , "type" , "Error");
            cJSON_AddStringToObject(error , "content" , "Token is not correct");
            res = cJSON_PrintUnformatted(error);
    }
}

void join_channel_func()
{
    char token_check[100];
    char channel_name[100];
    char htgt[100]; // file way
    sscanf(req , "%*s%*s%s%s" , channel_name , token_check);
    int index;
    int check=0;
    for (int j=0 ; j<=i ; j++)
    {
        if (strncmp(users[j].token , token_check , 32)==0)
        {
            check=1;
            index=j;
            break;
        }
    }
    if (check==1)
    {
        strcpy(htgt , "Resources/Channels/");
        strcat(htgt , channel_name);
        strcat(htgt , ".json");
        FILE* user;
        if ((user=fopen(htgt , "r"))!=NULL)
        {
            char channel_content[10000];
            fscanf(user , "%[^\n]s" , channel_content);
            fclose(user);
            user=fopen(htgt , "w");
            cJSON* json_content = cJSON_Parse(channel_content);
            cJSON* json_message = cJSON_GetObjectItem(json_content , "messages");
            cJSON* a;
            cJSON_AddItemToArray(json_message , a=cJSON_CreateObject());
            char name[100];
            strcpy(name , users[index].name);
            strcat(name , " Joined -");
            strcat(name , channel_name);
            strcat(name , "-");
            strcpy(users[index].channel , channel_name);
            cJSON_AddStringToObject(a , "sender" , "Server");
            cJSON_AddStringToObject(a , "content" , name);
            cJSON* bracket= cJSON_CreateObject();
            cJSON_AddItemToObject(bracket , "messages" , json_message);
            char* json_channel = cJSON_PrintUnformatted(bracket);
            //printf("%s\n" , jsonchannel);
            fprintf(user , "%s" , json_channel);
            cJSON* success=cJSON_CreateObject();
            cJSON_AddStringToObject(success , "type" , "Successful");
            cJSON_AddStringToObject(success , "content" , "");
            res = cJSON_PrintUnformatted(success);
            fclose(user);
        }
        else
        {
            cJSON* error = cJSON_CreateObject();
            cJSON_AddStringToObject(error , "type" , "Error");
            cJSON_AddStringToObject(error , "content" , "This channel dosen't exist");
            res = cJSON_PrintUnformatted(error);
        }
    }
    else
    {
        cJSON* error = cJSON_CreateObject();
        cJSON_AddStringToObject(error , "type" , "Error");
        cJSON_AddStringToObject(error , "content" , "Token is not correct");
        res = cJSON_PrintUnformatted(error);
    }
}

void send_message_func()
{
    char token_check[100];
    char message[100];
    char htgt[100]; // file way
    int k=0;
    sscanf(req , "%*s%*c%[^,]s" , message , token_check);
    for (int i=strlen(req)-33 ; i<strlen(req)-1 ; i++)
    {
        token_check[k]=req[i];
        k++;
    }
    token_check[k]=0;
    //printf("--%s------%s-----%s--------\n",req,message,token_check);
    int index;
    int check=0;
    for (int j=0 ; j<=i ; j++)
    {
        if (strncmp(users[j].token , token_check , 32)==0)
        {
            check=1;
            index=j;
            break;
        }
    }
    if (check==1)
    {
        strcpy(htgt , "Resources/Channels/");
        strcat(htgt , users[index].channel);
        strcat(htgt , ".json");
        FILE* user;
        user=fopen(htgt , "r");
        char channel_content[10000];
        fscanf(user , "%[^\n]s" , channel_content);
        fclose(user);
        user=fopen(htgt , "w");
        cJSON* json_content=cJSON_Parse(channel_content);
        cJSON* json_message=cJSON_GetObjectItem(json_content , "messages");
        cJSON* a;
        cJSON_AddItemToArray(json_message , a=cJSON_CreateObject());
        char name[100];
        strcpy(name , users[index].name);
        cJSON_AddStringToObject(a , "sender" , name);
        cJSON_AddStringToObject(a , "content" , message);
        cJSON* bracket= cJSON_CreateObject();;
        cJSON_AddItemToObject(bracket , "messages" , json_message);
        char* json_channel = cJSON_PrintUnformatted(bracket);
        printf("%s\n" , json_channel);
        fprintf(user , "%s" , json_channel);
        cJSON* success=cJSON_CreateObject();
        cJSON_AddStringToObject(success , "type" , "Successful");
        cJSON_AddStringToObject(success , "content" , "");
        res = cJSON_PrintUnformatted(success);
        fclose(user);
    }
    else
    {
        cJSON* error = cJSON_CreateObject();
        cJSON_AddStringToObject(error , "type" , "Error");
        cJSON_AddStringToObject(error , "content" , "Token is not correct");
        res = cJSON_PrintUnformatted(error);
    }
}

void channel_memebers_func()
{
    char token_check[100];
    char htgt[100]; //file way
    sscanf(req , "%*s%*s%s" , token_check);
    int index;
    int check=0;
    for (int j=0 ; j<=i ; j++)
    {
        if (strncmp(users[j].token , token_check , 32)==0)
        {
            check=1;
            index=j;
            break;
        }
    }
    if (check==1)
    {
        cJSON* bracket=cJSON_CreateObject();
        cJSON* member_list=cJSON_CreateArray();
        for (int j=0;j<=i;j++)
        {
            if(strcmp(users[j].channel , users[index].channel)==0)
            cJSON_AddItemToArray(member_list , cJSON_CreateString(users[j].name));
        }
        cJSON_AddStringToObject(bracket , "type" , "List");
        cJSON_AddItemToObject(bracket , "content" , member_list);
        res=cJSON_PrintUnformatted(bracket);
    }
    else
    {
        cJSON* error=cJSON_CreateObject();
        cJSON_AddStringToObject(error , "type" , "Error");
        cJSON_AddStringToObject(error , "content" , "Token is not correct");
        res=cJSON_PrintUnformatted(error);
    }
}

void refresh_func()
{
    char token_check[100];
    char htgt[100]; //file way
    sscanf(req , "%*s%s" , token_check);
    int index;
    int check=0;
    for (int j=0 ; j<=i ; j++)
    {
        if (strncmp(users[j].token , token_check , 32)==0)
        {
            check=1;
            index=j;
            break;
        }
    }
    if (check==1)
    {
        strcpy(htgt , "Resources/Channels/");
        strcat(htgt , users[index].channel);
        strcat(htgt , ".json");
        FILE* user;
        char channel_content[10000];
        memset(channel_content , 0 , sizeof(channel_content));
        user=fopen(htgt , "r");
        fscanf(user , "%[^\n]s" , channel_content);
        fclose(user);
        cJSON* json_channel_content=cJSON_CreateArray();
        cJSON* saver=cJSON_Parse(channel_content);
        cJSON* inbracket=cJSON_GetObjectItem(saver , "messages");
        for (int j=previous_arraaynum ; j<cJSON_GetArraySize(inbracket) ; j++)
        {
            if (j==previous_arraaynum)
            cJSON_AddItemToArray(json_channel_content , cJSON_GetArrayItem(inbracket , j));
        }
        previous_arraaynum=cJSON_GetArraySize(inbracket);
        cJSON* bracket=cJSON_CreateObject();
        cJSON_AddStringToObject(bracket , "type" , "List");
        cJSON_AddItemToObject(bracket , "content" , json_channel_content);
        res=cJSON_PrintUnformatted(bracket);
    }
    else
    {
        cJSON* error=cJSON_CreateObject();
        cJSON_AddStringToObject(error , "type" , "Error");
        cJSON_AddStringToObject(error , "content" , "Token is not correct");
        res=cJSON_PrintUnformatted(error);
    }
}

void leave_channel_func()
{
    char token_check[100];
    char htgt[100]; //file way
    sscanf(req , "%*s%s" , token_check);
    int index;
    int check=0;
    for (int j=0 ; j<=i ; j++)
    {
        if (strncmp(users[j].token , token_check , 32)==0)
        {
            check=1;
            index=j;
            break;
        }
    }
    if (check==1)
    {
        strcpy(htgt , "Resources/Channels/");
        strcat(htgt , users[index].channel);
        strcat(htgt , ".json");
        FILE* user;
        user=fopen(htgt , "r");
        char channel_content[10000];
        fscanf(user , "%[^\n]s" , channel_content);
        fclose(user);
        user=fopen(htgt , "w");
        cJSON* json_content = cJSON_Parse(channel_content);
        cJSON* json_message = cJSON_GetObjectItem(json_content,"messages");
        cJSON* a;
        cJSON_AddItemToArray(json_message , a=cJSON_CreateObject());
        char name[1000];
        strcpy(name , users[index].name);
        strcat(name , " Left -");
        strcat(name , users[index].channel);
        strcat(name,"-");
        cJSON_AddStringToObject(a , "sender" , "Server");
        cJSON_AddStringToObject(a , "content" , name);
        cJSON* bracket= cJSON_CreateObject();
        cJSON_AddItemToObject(bracket , "messages" , json_message);
        char* json_channel = cJSON_PrintUnformatted(bracket);
        fprintf(user , "%s" , json_channel);
        cJSON* success=cJSON_CreateObject();
        cJSON_AddStringToObject(success , "type" , "Successful");
        cJSON_AddStringToObject(success , "content" , "");
        res=cJSON_PrintUnformatted(success);
        memset(users[index].channel , 0 , sizeof(users[index].channel));
        fclose(user);
    }
    else
    {
        cJSON* error = cJSON_CreateObject();
        cJSON_AddStringToObject(error , "type" , "Error");
        cJSON_AddStringToObject(error , "content" , "Token is not correct");
        res = cJSON_PrintUnformatted(error);
    }
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server;
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
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");

    // Now server is ready to listen and verify
    while (1){
        memset(req,0,sizeof(req));
        recvreq(server_socket);
    }

}
