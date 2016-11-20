#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <curl/curl.h>

#define SERVER "10.21."
#define DOT "."
#define SERVER_PORT 28900
#define CLIENT_SERVER_BUFFER 1024
#define Message "Who are you?\n"
#define URL "http://pilot.westmont.edu:28900?i="

/* Helper Functions */

// Assorted Variables
int fd, ret;
char *buffer, *name;


//String Concatanator
char* concatStr (char *str0, char *str1) {
  char *out = malloc(strlen(str0) + strlen(str1) + 1);

  strcpy(out, str0);
  strcat(out, str1);

  return out;
}


//Connecting
int connectToServer(char* server){
	struct sockaddr_in sin;	

	fd = socket(AF_INET, SOCK_STREAM, 0); // establish socket, IPv4, TCP
	inet_pton(AF_INET, server, &sin.sin_addr); // convert network address
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);

	return(connect(fd, (struct sockaddr *) &sin, sizeof(sin)));
}


//Score 
// http://stackoverflow.com/questions/2523467/how-to-split-a-string-to-2-strings-in-c
int sendScore(){
	char* score_Url, tofree, string, first, second;
	CURL *curl = curl_easy_init();

	string = *buffer;
	tofree = string;

	for (int i = 0; i < 2; ++i)
	{
		if(i == 0){
			first = strsep(&string, " ");
		} 
		if (i == 1){
			second = strsep(&string, "\n");
		}
	}
	free(tofree);

	// Create URl
	score_Url = concatStr(URL, name);
	score_Url = concatStr(score_Url, "&u=");
	score_Url = concatStr(score_Url, first);
	score_Url = concatStr(score_Url, "&where=");
	score_Url = concatStr(score_Url, second);

  curl_easy_setopt(curl, CURLOPT_URL, score_Url); // get request to URL
  curl_easy_perform(curl);
}


//Ip Scanner
char* ipScanner(){
	char* first, second, IPadd;

	for (int i = 0; i <= 255; ++i)
	{
		for (int j = 0; j <= 255; ++j)
		{
			first = itoa(i);
			second = itoa(j);
			IPadd = concatStr(SERVER, first);
			IPadd = concatStr(SERVER, DOT);
			IPadd = concatStr(SERVER, second);

			if(connectToServer(IPadd) > 0){
				send(fd, Message, strlen(Message)+1, 0);
				buffer = malloc(CLIENT_SERVER_BUFFER+1);
				ret = recv(fd, buffer, CLIENT_SERVER_BUFFER, 0);
				sendScore();
			}
		}
	}
}




/* Main Program */

int main(int argc, char *argv[])
{
	int len, stdinret;
	struct sockaddr_in sin;	
	char *stdinBuf;
	struct timeval tv;
	fd_set rfds;


	if(argc < 2){
		printf("Usage: ICU-client <screenname>\n");
		exit(1);
	}
	name = argv[1];

	while(1){
		ipScanner();
	}
		
}

    