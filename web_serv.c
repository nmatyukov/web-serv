#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include "send.h"
#include "request_process.h" 


int main(int argc, char *argv[])
{
  int sock, pid, ch_conf;
  FILE* conf;
  char buf_conf[4096];
  char * port_str;
  char * path;
  char * pos;
  int PORT;

//-----read_conf_file------------------------------

  conf=fopen("web_serv.conf","rb");
  
  if(conf==NULL){
      printf("Can't open web_serv.conf file");
      exit(1);
      }
  if (!fgets(buf_conf, sizeof(buf_conf), conf)) return -1; 
 
  port_str=strtok(buf_conf, " ");
  path=strtok(NULL, " "); 
  pos=strchr(path, '\n');
  if(pos)
  path[pos-path]=0;

  PORT=atoi(port_str);

  fclose(conf);

//-----socket----------------------------------------

  struct sockaddr_in serv_addr, clnt_addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(PORT);
  bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

  listen(sock, 50);
  printf("HTTP server listening on port %d at %s\n", PORT, inet_ntoa(serv_addr.sin_addr));

  while (1)
  {
    int s;
    FILE *f;

    s = accept(sock, NULL, NULL);
    
    if (s<0) break;
    
    if((pid=fork())==0){
        
        close(sock);
        f = fdopen(s, "r+");

        process(f, s, path);             //-------Our Function---------
        
        fclose(f);
        close(s);
        exit(0);
    }

   close(s);

  }

  close(sock);
  return 0;
}

