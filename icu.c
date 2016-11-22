#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include "/usr/local/unp/lib/unp.h"

#define PORT 28900
#define BUFLEN 1024

int sec = 0; //for incrim to 60sec

char* concatStr (char *str0, char *str1) {
  char *out = malloc(strlen(str0) + strlen(str1) + 1);

  strcpy(out, str0);
  strcat(out, str1);

  return out;
}

//every 60 seconds sends a ping (when select timeout)
void minping(char* name){
	char* url[1000];
	CURL *curl = curl_easy_init();
//if ++sec=60 send set sec to 0
	if(++sec > 59){
		//send notif
		sprintf(url, "http://pilot.westmont.edu:28900?i=%s&uptime=%s", name, sec);
		curl_easy_setopt(curl, CURLOPT_URL, url); //ping 4 points
 		curl_easy_perform(curl);
		sec = 0;
	}
}

int main (int argc, char * argv[]) {
  int sfd, len, sret;
  struct sockaddr_in sin; 
  char *username, *location, *toSend, *recvClient, *buffer;
  struct timeval tv = malloc(sizeof(timeval)); //global?
  fd_set readfds;
tv.tv_sec = 1;
tv.tv_usec = 0; 


  if(argc < 2)
  {
    printf("Usage: SERVERNAME <username> LOCATION <location>\n");
    exit(1);
  }
  username = argv[1];
  location = argv[2];

  toSend = concatStr(username, " ");
  toSend = concatStr(toSend, location);
  toSend = concatStr(toSend, "\n");
  buffer = malloc(strlen(toSend) + 1);
 
  sfd = Socket(AF_INET, SOCK_STREAM, 0); // establish socket, IPv4, TCP
  //readfds/setting up set
  FD_ZERO(&readfds);
  FD_SET(sfd, &readfds); //so that it adds the socket to the set
  FD_SET(fileno(stdin), &readfds); //a 2nd that we can use the time out on
  int maxfds = 4;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(PORT);

  Bind(&sfd, (struct sockaddr*)&sin, sizeof(&sin));

  Listen(&sfd, 1);

  while (1) {
   //select on fds
   int stime = select(maxfds, &readfds, NULL, NULL, NULL);
   if(stime < 0){
	exit(EXIT_FAILURE);
   }
   if(FD_ISSET(sfd, &readfds)){
	minping(username);
   }
    
    int cfd;
    struct sockaddr_in caddr;

    recvClient

    cfd = Accept(&sfd, (struct sockaddr*)&caddr, sizeof(&caddr));

    Send(&cfd, buffer, toSend, 0);
  }
}
