#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <getopt.h>
#include <unistd.h>

#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
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
    printf(c);
    switch (c) {
      case 0:
        switch (option_index)
        {
          case 0:
            servPort = atoi(optarg);
            printf(servPort);
            break;
          case 1:
            bufferSize = atoi(optarg);
            printf(bufferSize);
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
  int fd;
  int nread;
  char buf[bufferSize];
  struct sockaddr_in servaddr;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  servaddr.sin_port = htons(servPort);

  if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
    perror("connect");
    exit(1);
  }

  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, bufferSize)) > 0) {
    if (write(fd, buf, nread) < 0) {
      perror("write");
      exit(1);
    }
  }

  close(fd);
  exit(0);
}
