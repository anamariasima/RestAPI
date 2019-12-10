#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, const char *url, char *url_params, const char* header, char* cookie)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    /*
        PAS 1: Scriem numele metodei, calea, parametri din url (daca exista) si tipul protocolului
    */
    if (url_params != NULL)
    {
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    }
    else
    {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    /*
        PAS 2: Adaugam host-ul
    */
    sprintf(line, "HOST: %s", host);
    compute_message(message, line);

    /*
        PAS 3: Adaugam headere si/ sau cookies, respectand forma protocolului
    */
        
        if(header != NULL) {
            sprintf(line, "Authorization: Bearer %s", header);
            compute_message(message, line);
        }
        if(cookie != NULL) {
			sprintf(line, "Cookie: %s", cookie);
            compute_message(message, line);
        }
        
    
    /*
        PAS 4: Adaugam linia de final
    */
    strcpy(line, "\r\n");
    strcat(message, line);
    free(line);
    
    return message;
}

char *compute_post_request(char *host, const char *url, char *form_data, const char*header, char* cookie, const char* type)
{

    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    /*
        PAS 1: Scriem numele metodei, calea si tipul protocolului
    */
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
  

    /*
        PAS 2: Adaugam host-ul
    */
    sprintf(line, "HOST: %s", host);
    compute_message(message, line);
    

    /*
        PAS 3: Adaugam headere si/ sau cookies, respectand forma protocolului
    */
    if(header != NULL) {
        sprintf(line, "Authorization: Bearer %s", header);
        compute_message(message, line);
    }
    
    if(cookie != NULL) {

            
            sprintf(line, "Cookie: %s", cookie);
            compute_message(message, line);
           /* sprintf(line, "Cookie: %s", cookie2);
            compute_message(message, line); */
    }
    
    sprintf(line, "Content-Length: %ld", strlen(form_data));
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", type);
    compute_message(message, line);
    

    /*
        PAS 4: Adaugam linia de final de antent
    */
    strcpy(line, "\r\n");
    strcat(message, line);
  

    /*
        PAS 5: Adaugam data
    */
    strcpy(line, form_data);
    compute_message(message, line);
    free(line);
   
    return message;
}