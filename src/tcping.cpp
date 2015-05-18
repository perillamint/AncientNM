#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <netinet/in.h>

#include "ipsock.h"
#include "ipv4sock.h"
#include "tcping.h"

struct tcping_threadarg {
  int ipver;
  char *ifname;
  char *targetaddr;
  int port;
  IPsock* ipsock;
} threadarg;

TCPing::TCPing(int ipver, const char *ifname, const char *targetaddr, int port) {
  switch (ipver) {
  case 4:
    this -> ipsock = (IPsock*)new IPV4sock(ifname);
    break;
  case 6:
    //TODO: IPv6 support.
    break;
  default:
    //ERROR it.
    break;
  }

  //TODO: Do DNS lookup
  //TODO: build TCP headers.
}

TCPing::~TCPing() {
  delete this -> ipsock;
}

int TCPing::init_ping() {
  //Open socket

  char addr[20];
  
  this -> ipsock -> open_socket("8.8.8.8");
  this -> ipsock -> getipaddr(addr);

  printf("%s\n", addr);

  return 0;
}

int TCPing::start_ping() {
  //Start new thread and send ping
  //Since, bionic and uClibc doesn't support C++ libs,
  //We have to use pthread instead of std::thread
  
  if(pthread_create(&this -> ping_pthread, NULL, this -> ping_thread, this -> ipsock)) {
    //Failed to create thread
  printf("Fail!");
    return -1;
  }

  pthread_join(this -> ping_pthread, NULL);

  return 0;
}

//TODO: non-static member function cannot passed into pthread_create.
//Make it static. (get this from arg? maybe.)
void* TCPing::ping_thread(void *arg) {
  IPsock *ipsock = (IPsock*) arg;
  //Private function - ping thread.
  //since send_packet is MT-Safe, there is no lock needed for
  //sending packet.

  //Just for testing

  printf("Ping!");
  ipsock -> send_packet(IPPROTO_TCP, 64, 80, "HELLOIP", 8);
  //
}
