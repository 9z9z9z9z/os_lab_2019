#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SADDR struct sockaddr

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

  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  char buf[bufferSize];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  
  if (servPort <= 0 || bufferSize <= 0) {
    fprintf(stderr, "Using: %s --SERV_PORT 10050 --BUFSIZE 100\n", argv[0]);
  }

    if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      exit(1);
    }

    memset(&servaddr, 0, kSize);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(servPort);

    if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
      perror("bind");
      exit(1);
    }

    if (listen(lfd, 5) < 0) {
      perror("listen");
      exit(1);
    }

    while (1) {
      unsigned int clilen = kSize;

      if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
        perror("accept");
        exit(1);
      }
      printf("connection established\n");

      while ((nread = read(cfd, buf, bufferSize)) > 0) {
        write(1, &buf, nread);
      }

      if (nread == -1) {
        perror("read");
        exit(1);
      }
      close(cfd);
    }
}
