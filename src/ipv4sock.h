#ifndef __IPV4SOCK_H_
#define __IPV4SOCK_H_

#include "ipsock.h"

class IPV4sock : public IPsock {
 public:
  IPV4sock(const char *ifname);
  ~IPV4sock();
  int open_socket(const char *destaddr);
  int close_socket();
  int getipaddr(char *addr);
  int send_packet(uint8_t proto, uint8_t ttl, uint16_t destport, const void *pktbody, int pktbodysize);
 private:
  int sockfd;
  int sockcnt;
  int sockflag;
  int header_len;
  char destaddr[16];
  struct iphdr *iphdr;
  struct ifreq *ifreq;
};

#endif
