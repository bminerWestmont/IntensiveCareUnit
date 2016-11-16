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

char* concatStr (char *str0, char *str1) {
  char *out = malloc(strlen(str0) + strlen(str1) + 1);

  strcpy(out, str0);
  strcat(out, str1);

  return out;
}

int main (int argc, char * argv[]) {
  int sfd, len, sret;
  struct sockaddr_in sin; 
  char *username, *location, *toSend, *recvClient, *buffer;
  struct timeval tv;
  fd_set readfds;

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

  sin.sin_family = AF_INET;
  sin.sin_port = htons(PORT);

  Bind(&sfd, (struct sockaddr*)&sin, sizeof(&sin));

  Listen(&sfd, 1);

  while (1) {
    int cfd;
    struct sockaddr_in caddr;

    recvClient

    cfd = Accept(&sfd, (struct sockaddr*)&caddr, sizeof(&caddr));

    Send(&cfd, buffer, toSend, 0);
  }
}
