#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024
#define KEY_SIZE 1024

unsigned char* keyRead() {
    static unsigned char key[KEY_SIZE];
    FILE* key_file;

    // Open key file
    key_file = fopen("key.txt", "rb");
    if (key_file == NULL) {
        printf("Error opening key file\n");
        exit(0);
    }

    // Read key from file
    fread(key, 1, KEY_SIZE, key_file);

    // Close key file
    fclose(key_file);

    return key;
}


void send_file(FILE *fp, int sockfd, unsigned char* key){
  int n;
  char data[SIZE] = {0};
  int i;

  while(fgets(data, SIZE, fp) != NULL) {
    // Pad data with null bytes to match key length
    int len = strlen(data);
    for (i = len; i < KEY_SIZE; i++) {
      data[i] = 0;
    }
    // XOR data with key
    for (i = 0; i < KEY_SIZE; i++) {
      data[i] ^= key[i];
    }
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}


int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;
  static unsigned char key2[KEY_SIZE];
  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;
  char *filename = "send.txt";



  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
	printf("[+]Connected to Server.\n");

  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

  send_file(fp, sockfd, (keyRead()));
  printf("[+]File data sent successfully.\n");

	printf("[+]Closing the connection.\n");
  close(sockfd);

  return 0;
}