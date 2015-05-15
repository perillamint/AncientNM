#ifndef __IPV4SOCK_H_
#define __IPV4SOCK_H_

#include "socket.h"

class IPV4socket : public Socket {
 public:
  IPV4sock(char *ifname);
 private:
  int sockfd;
  int header_len;
  struct iphdr *packet_header;
  struct ifreq *ifreq;
}

#endif
