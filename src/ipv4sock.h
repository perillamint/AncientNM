#ifndef __IPV4SOCK_H_
#define __IPV4SOCK_H_

#include "ipsock.h"

class IPV4sock : public IPsock {
 public:
  IPV4sock(char *ifname);
  ~IPV4sock();
  int open_socket();
  int close_socket();
  int send_packet(uint8_t proto, uint8_t ttl, char* destaddr, uint16_t destport, void *pktbody, int pktbodysize);
 private:
  int sockfd;
  int header_len;
  struct iphdr *iphdr;
  struct ifreq *ifreq;
};

#endif
