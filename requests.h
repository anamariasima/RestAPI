#ifndef _REQUESTS_
#define _REQUESTS_

char *compute_get_request(char *host,const char *url, char *url_params,const char* header, char* cookie);
char *compute_post_request(char *host,const char *url, char *form_data, const char* header, char* cookie, const char* type);

#endif