#ifndef __SOCKET_H_
#define __SOCKET_H_

//Abstract class
class IPsock {
 public:
  virtual int open_socket() = 0;
  virtual int close_socket() = 0;
  virtual int send_packet(uint8_t proto, uint8_t ttl, char* destaddr, uint16_t destport, void *pktbody, int pktbodysize) = 0;
};

#endif
