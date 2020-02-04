#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cJSON
{
    char type[100][100];
    char content[100][100];
    char* valuestring;
    struct cJSON* next[100];
    int hmn; //how many next...this is for counting nexts :)
};
typedef struct cJSON cJSON;

int what_to_add=0;

cJSON* cJSON_CreateObject()
{
    cJSON* object=(cJSON*) malloc(sizeof(cJSON));
    memset(object , 0 , sizeof(object));
    return object;
}

cJSON* cJSON_CreateArray()
{
    cJSON* cJSON_array=(cJSON*) malloc(sizeof(cJSON));
    memset(cJSON_array , 0 , sizeof(cJSON_array));
    return cJSON_array;
}

cJSON* cJSON_CreateString(char* cJSON_string)
{
    cJSON* object=(cJSON*) malloc(sizeof(cJSON));
    memset(object , 0 , sizeof(object));
    strcpy(object->valuestring , cJSON_string);
    return object;
}

void cJSON_AddStringToObject(cJSON* head , char* type_pointer , char* content_pointer) //this is just like add item to object which in this case item is a single object
{
    if (what_to_add==0)  //this will separate two kinds of type_pointer  user name/password or sender/content or type/content
    {
        strcpy((head->type[0]) , type_pointer);
        strcpy((head->content[0]) , content_pointer);
        what_to_add=1;
    }
    else
    {
        strcpy((head->type[1]) , type_pointer);
        strcpy((head->content[1]) , content_pointer);
        what_to_add=0;
    }
}

cJSON* cJSON_GetObjectItem(cJSON* object , char* item)
{
    cJSON* object_item=(cJSON*)malloc(sizeof(cJSON));
    memset(object_item , 0 , sizeof(object_item));
    for (int i=0 ; i<2 ; i++)
    {
        if (strcmp(item , object->type[i])==0)
            strcpy(object_item->valuestring , object->content[i]);
    }
    return object_item;
}

cJSON* cJSON_GetArrayItem(cJSON* cJSON_array , int i)
{
    cJSON* object_item=(cJSON*)malloc(sizeof(cJSON));
    memset(object_item , 0 , sizeof(object_item));
    object_item=cJSON_array->next[i];
    return object_item;
}

void cJSON_AddItemToArray(cJSON* cJSON_array , cJSON* item)
{
    (cJSON_array->next[cJSON_array->hmn])=item;
    (cJSON_array->hmn)++;
}

void cJSON_AddItemToObject(cJSON* object , char* text , cJSON* item)
{
    strcpy(object->type[0] , text);
    (object->next[0])=item;
    (object->hmn)++;
    for (int i=0 ; i<= (item->hmn) ; i++)
    {
        object->next[i]=item->next[i-1];
        (object->hmn)++;
    }
}

int cJSON_GetArraySize(cJSON* cJSON_array)
{
    int size=0;
    while (cJSON_array->next[size]!=NULL) size++;
    return size;
}

cJSON* cJSON_Parse2(char* text , int* i)
{
    cJSON* ans;
    if (text[(*i)]='{')
    {
        ans=cJSON_CreateObject();
        cJSON* temp;
        (*i)++;
        int index=0;
        if (text[(*i)]!='}')
        {
            temp=cJSON_Parse2(text , i);
            (ans->next[index])=temp;
            index++;
        }
        while (text[(*i)]!='}')
        {
            (*i)++;
            temp->next[index]=cJSON_Parse2(text , i);
            temp=temp->next[index];
            index++;
        }
        (*i)++;
    }
    else if(text[(*i)]=='[')
    {
        ans=cJSON_CreateArray();
        cJSON* temp;
        i++;
        int index=0;
        if (text[(*i)]!=']')
        {
            temp=cJSON_Parse2(text , i);
            ans->next[index]=temp;
            index++;
        }
        while (text[(*i)]!=']')
        {
            (*i)++;
            temp->next[index]=cJSON_Parse2(text , i);
            temp=(temp->next[index]);
            index++;
        }
        (*i)++;
    }
    else if (text[(*i)]=='"')
    {
        (*i)++;
        int k=0;
        char temp_Char[100];
        while ((text[(*i)])!='"')
        {
            (*i)++;
            k++;
            temp_Char[k]=text[(*i)];
        }
        temp_Char[k-1] = 0;
        if (text[(*i)]==':')
        {
            (*i)++;
            ans= cJSON_Parse2(text , i);
            strcpy(ans->type[0] , temp_Char);
        }
        else
        {
            ans=cJSON_CreateString(temp_Char);
        }
    }
    return ans;
}

cJSON* cJSON_Parse(char* text)
{
    int* i=(int*)malloc(sizeof(int));
    memset(i , 0 , sizeof(i));
    cJSON* ans=cJSON_Parse2(text , i);
    return ans;
}

char* cJSON_PrintUnformatted(cJSON* object)
{

}

int main()
{

}
