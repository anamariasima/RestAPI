#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "requests.h"
#include "helpers.h"
#include "parson.h"

int main(int argc, char *argv[])
{
    
    char *message;
    char *response;
    int sockfd;
    JSON_Value *root_value;
    JSON_Object *element, *data, *queryParams;
    const char *url = NULL;
    const char *type = NULL;
    const char *jwt = NULL;
  	
  	
    //task1
    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("185.118.200.35", "/task1/start", NULL, NULL, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(message);
    puts(response);
    close_connection(sockfd);


    //task2
    int indice = 0;
   	char *linii[100]; //o sa salvez intr-un vector liniile(delimitate de "\n") din raspuns
   	                  //obtinerea vectorului de linii
   	char *token = strtok(response, "\n");
   	int i = 0;
   	while(token != NULL) {
   	    linii[i] = token;
   	    i++;
   	    token = strtok(NULL, "\n");
        indice++; //numarul de linii
   	}
    char* info_json;
    if(strncmp(linii[indice - 1], "{",1) == 0) {  
	      info_json = linii[indice - 1]; //mesajul(body-ul) este ultima parte din raspuns
	  }

   	root_value = json_parse_string(info_json); //mesajul de parsat
   	element = json_value_get_object(root_value); 
   	const char *user, *passwd;
    url = json_object_get_string(element, "url");
    data = json_object_get_object(element, "data");
   	user = json_object_get_string(data, "username");
   	passwd = json_object_get_string(data, "password");
   	type = json_object_get_string(element, "type");
    
    char* cookie[700];//cookie-urile le salvez intr-un vector de string-uri, iar apoi le
                      //concatenez pentru a pune toate cookie-urile in header-ul de Cookie din request 
    int length = 0;//numarul de cookie-uri
    int j = 0;
    //obtinere cookie
   	for(i = 0; i < indice; i++) {
   	    if(strncmp(linii[i], "Set-Cookie", 10) == 0) {
   			  cookie[j] = strtok(linii[i], " ");
    		  cookie[j] = strtok(NULL, ";");
    		  j++;
    		  length++;
    	  }
   	} 
   	char final_cookie[700];//o sa contina concatenarea cookie-urilor
   	strcpy(final_cookie, cookie[0]);
   	//concatenare cookie-uri
   	for(i = 1; i < length; i++) {
   	    strcat(final_cookie, "; ");
   	    strcat(final_cookie, cookie[i]);
   	}
    //concatenez informatiile pe care sa le adaug la cerera de post la form_data
   	char string[700];
   	strcpy(string, "username=");
   	strcat(string, user);
   	strcat(string, "&password=");
   	strcat(string, passwd);
   	
   	sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("185.118.200.35", url, string, NULL, final_cookie, type);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(message);
    puts(response);
    close_connection(sockfd);
    
    //task3
    indice = 0;
	  token = strtok(response, "\n");
   	i = 0;
    //obtinerea vectorului de linii
   	while(token != NULL) {
   	    linii[i] = token;
   	    i++;
   	    token = strtok(NULL, "\n");
   	    indice++; //numarul de linii
   	}
    
    if(strncmp(linii[indice - 1], "{",1) == 0) {  
        info_json = linii[indice - 1]; //mesajul(body-ul) este ultima parte din raspuns
    }
    
    root_value = json_parse_string(info_json);
   	element = json_value_get_object(root_value);
    const char *id;
    url = json_object_get_string(element, "url");
    data = json_object_get_object(element, "data");
   	jwt = json_object_get_string(data, "token");
    queryParams = json_object_get_object(data, "queryParams");
    id = json_object_get_string(queryParams, "id");
    //concatenez informatiile pe care le adaug la cererea de get la url_params
    strcpy(string, "raspuns1=omul&raspuns2=numele&id=");
    strcat(string, id);
    length = 0;
    j = 0;
    //obtinere cookie
   	for(i = 0; i < indice; i++) {
   	    if(strncmp(linii[i], "Set-Cookie", 10) == 0) {
            cookie[j] = strtok(linii[i], " ");
    		    cookie[j] = strtok(NULL, ";");
    		    j++;
    		    length++;
    	  }
   	} 
   	strcpy(final_cookie, cookie[0]);
   	//concatenare cookie-uri
   	for(i = 1; i < length; i++) {
   		strcat(final_cookie, "; ");
   		strcat(final_cookie, cookie[i]);
   	}
    
    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("185.118.200.35", url, string, jwt, final_cookie);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(message);
    puts(response);
    close_connection(sockfd);
    
    //task4
    indice = 0;
	  token = strtok(response, "\n");
   	i = 0;
    //obtinerea vectorului de linii
   	while(token != NULL) {
   	    linii[i] = token;
   	    i++;
   	    token = strtok(NULL, "\n");
   	    indice++; //numarul de linii
   	}
    
	  if(strncmp(linii[indice - 1], "{",1) == 0) {  
        info_json = linii[indice - 1]; //mesajul(body-ul) este ultima parte din raspuns
     }
    

    root_value = json_parse_string(info_json);
   	element = json_value_get_object(root_value);
    url = json_object_get_string(element, "url");
    length = 0;
    j = 0;
    //obtinere cookie
   	for(i = 0; i < indice; i++) {
   	    if(strncmp(linii[i], "Set-Cookie", 10) == 0) {
   	        cookie[j] = strtok(linii[i], " ");
            cookie[j] = strtok(NULL, ";");
            j++;
            length++;
    	  }
   	} 
   	strcpy(final_cookie, cookie[0]);
   	//concatenare cookie-uri
   	for(i = 1; i < length; i++) {
   	    strcat(final_cookie, "; ");
   	    strcat(final_cookie, cookie[i]);
   	}

   	sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("185.118.200.35", url, NULL, jwt, final_cookie);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(message);
    puts(response);
    close_connection(sockfd);
  
    
    //task 5    
	  //partea 1
	  indice = 0;
	  token = strtok(response, "\n");
   	i = 0;
    //obtinerea vectorului de linii
   	while(token != NULL) {
   	    linii[i] = token;
   	    i++;
   	    token = strtok(NULL, "\n");
   	    indice++; //numarul de linii
   	}
    
    if(strncmp(linii[indice - 1], "{",1) == 0) {  
        info_json = linii[indice - 1]; //mesajul(body-ul) este ultima parte din raspuns
    }
    

    root_value = json_parse_string(info_json);
   	element = json_value_get_object(root_value);
    const char *q, *APPID, *data_url, *api, *url_post;
    char* host;
    type = json_object_get_string(element, "type");
    url_post = json_object_get_string(element, "url");
    data = json_object_get_object(element, "data");
    data_url = json_object_get_string(data, "url");
    queryParams = json_object_get_object(data, "queryParams");
    q = json_object_get_string(queryParams, "q");
    APPID = json_object_get_string(queryParams, "APPID");
    url_post = json_object_get_string(element, "url");
    
    strcpy(url, strstr(data_url, "/"));
	  api = strtok(data_url, "/");

	  //iau ip-ul 
    struct hostent *ip = gethostbyname(api);
    struct in_addr **adrese =  (struct in_addr **)ip->h_addr_list;
    host = inet_ntoa(*adrese[1]);

    //concatenez informatia pe care o adaug la cererea de get la url_params
    strcpy(string, "q=");
    strcat(string, q);
    strcat(string, "&APPID=");
    strcat(string, APPID);
    
    length = 0;
    j = 0;
    //obtinere cookie
   	for(i = 0; i < indice; i++) {
   	    if(strncmp(linii[i], "Set-Cookie", 10) == 0) {
   	        cookie[j] = strtok(linii[i], " ");
            cookie[j] = strtok(NULL, ";");
            j++;
    		    length++;
        }
   	} 
   		
   	
   	strcpy(final_cookie, cookie[0]);
   	//concatenare cookie-uri
   	for(i = 1; i < length; i++) {
   		strcat(final_cookie, "; ");
   		strcat(final_cookie, cookie[i]);
   	}

    sockfd = open_connection(host, 80, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(host, url, string, NULL,  NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(message);
    puts(response);
    close_connection(sockfd);

    
    //partea 2
    indice = 0;
	  token = strtok(response, "\n");
   	i = 0;
    //obtinerea vectorului de linii
   	while(token != NULL) {
   		linii[i] = token;
   		i++;
   		token = strtok(NULL, "\n");
   		indice++; //numarul de linii
   	}
    
    if(strncmp(linii[indice - 1], "{",1) == 0) {  
        info_json = linii[indice - 1]; //mesajul(body-ul) este ultima parte din raspuns
    }
    
    root_value = json_parse_string(info_json);
   	element = json_value_get_object(root_value);

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("185.118.200.35", url_post, info_json, jwt, final_cookie,type);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(message);
    puts(response);


    return 0; 
}
