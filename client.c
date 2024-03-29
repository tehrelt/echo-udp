#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define BUFFER_LENGTH 512

int main(int argc, char **argv) {

  if (argc < 3) {
    printf("%s: usage %s <host> <port>\n", argv[0], argv[0]);
    exit(1);
  }

  struct sockaddr_in si_other;

  char *host = argv[1];
  int port = atoi(argv[2]);
  char buffer[BUFFER_LENGTH];

  int sockfd;
  socklen_t slen = sizeof(si_other);

  if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0))) {
    perror("socket");
    exit(1);
  }

  memset((char *)&si_other, 0, sizeof(si_other));

  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(port);
  si_other.sin_addr.s_addr = inet_addr(host);

  if (0 != connect(sockfd, (struct sockaddr *)&si_other, sizeof(si_other))) {
    perror("connect");
    exit(1);
  }

  buffer[0] = 15;

  if (-1 == sendto(sockfd, buffer, 4, 0, (struct sockaddr *)&si_other, slen)) {
    perror("sendto");
    exit(1);
  }

  if (-1 == recvfrom(sockfd, buffer, BUFFER_LENGTH, 0,
                     (struct sockaddr *)&si_other, &slen)) {
    perror("recvfrom");
    exit(1);
  }

  printf("successfully connected to %s:%d\n", host, port);

  while (1) {
    printf("Enter a message: ");
    fgets(buffer, BUFFER_LENGTH, stdin);

    if (-1 == sendto(sockfd, buffer, strlen(buffer), 0,
                     (struct sockaddr *)&si_other, slen)) {
      perror("sendto");
      exit(1);
    }

    memset(buffer, 0, BUFFER_LENGTH);
  }
}
