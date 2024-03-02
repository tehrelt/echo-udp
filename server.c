#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define BUFFER_LENGTH 512

int main(int argc, char **argv) {

  if (argc < 2) {
    printf("%s: usage: %s <port>\n", argv[0], argv[0]);
    exit(1);
  }

  int port;

  if (0 == (port = atoi(argv[1]))) {
    perror("atoi");
    exit(1);
  }

  struct sockaddr_in si, si_other;

  int s, i;
  socklen_t slen = sizeof(si_other);
  int recv_len;

  char buf[BUFFER_LENGTH];

  if (-1 == (s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))) {
    perror("socket");
    exit(1);
  }

  memset((char *)&si, 0, sizeof(si));
  si.sin_family = AF_INET;
  si.sin_port = htons(port);
  si.sin_addr.s_addr = htonl(INADDR_ANY);

  if (-1 == bind(s, (struct sockaddr *)&si, sizeof(si))) {
    perror("bind");
    exit(1);
  }

  printf("echo server started on %d port\n", port);

  while (1) {
    printf("waiting for packet...\n");
    fflush(stdout);

    if (-1 == (recv_len = recvfrom(s, buf, BUFFER_LENGTH, 0,
                                   (struct sockaddr *)&si_other, &slen))) {
      perror("recvfrom");
      exit(1);
    }

    printf("Data:\n%s\n", buf);
  }
}
