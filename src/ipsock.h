#ifndef __SOCKET_H_
#define __SOCKET_H_

#include <cstdint>

//Abstract class
class IPsock {
 public:
  virtual int open_socket(const char *addr) = 0;
  virtual int close_socket() = 0;
  virtual int getipaddr(char *addr) = 0;

  //NOTE: This method HAVE TO BE MT-SAFE.
  virtual int send_packet(uint8_t proto, uint8_t ttl, uint16_t destport, const void *pktbody, int pktbodysize) = 0;
};

#endif
