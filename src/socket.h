#ifndef __SOCKET_H_
#define __SOCKET_H_

//Abstract class
class Socket {
 public:
  int open_socket();
  int close_socket();
  int send_packet(uint8_t proto, uint8_t ttl, char* destaddr, uint16_t destport, void *pktbody, int pktbodysize);
}

#endif
