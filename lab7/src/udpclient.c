#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
  char *port;
  int servPort = -1;
  int bufferSize = -1;
  char *IP = "127.0.0.1";
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
            port = optarg;
            servPort = atoi(optarg);
            break;
          case 1:
            bufferSize = atoi(optarg);
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
  char sendline[bufferSize], recvline[bufferSize + 1];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(servPort);

  if (inet_pton(AF_INET, port, &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);

  while ((n = read(0, sendline, bufferSize)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }
    if (recvfrom(sockfd, recvline, bufferSize, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }
    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);
}
