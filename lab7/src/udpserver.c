#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
  int servPort = -1;
  int bufferSize = -1;
  int c = 2;
  while (c != -1) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"SERV_PORT", required_argument, 0, 0},
                                      {"BUFSIZE", required_argument, 0, 0},
                                      {0, 0, 0, 0}};
    int option_index = 0;
    c = getopt_long(argc, argv, "", options, &option_index);
    switch (c) {
      case 0:
        switch (option_index)
        {
        case 0:
          servPort = atoi(optarg);
              printf("port: %d\n", servPort);
          break;
        case 1:
          bufferSize = atoi(optarg);
              printf("buf: %d\n", bufferSize);
          break;
        default:
          printf("Index %d is out of options\n", option_index);
          break;
        }
        break;
      case '?':
        printf("Unknown argument\n");
        break;
      default:
        break;
    }
  }
  int sockfd, n;
  char mesg[bufferSize], ipadr[16];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  memset(&servaddr, 0, SLEN);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(servPort);

  if (bind(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
    perror("bind problem");
    exit(1);
  }
  printf("SERVER starts...\n");

  while (1) {
    unsigned int len = SLEN;

    if ((n = recvfrom(sockfd, mesg, bufferSize, 0, (SADDR *)&cliaddr, &len)) < 0) {
      perror("recvfrom");
      exit(1);
    }
    mesg[n] = 0;

    printf("REQUEST %s      FROM %s : %d\n", mesg,
           inet_ntop(AF_INET, (void *)&cliaddr.sin_addr.s_addr, ipadr, 16),
           ntohs(cliaddr.sin_port));

    if (sendto(sockfd, mesg, n, 0, (SADDR *)&cliaddr, len) < 0) {
      perror("sendto");
      exit(1);
    }
  }
}
