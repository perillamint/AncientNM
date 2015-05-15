#include <stdlib.h>
#include <pthread.h>

#include "ipsock.h"
#include "ipv4sock.h"
#include "tcping.h"

TCPing::TCPing(int ipver, char *ifname, char *targetaddr, int port) {
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
  this -> ipsock -> open_socket();

  return 0;
}

int TCPing::start_ping() {
  //Start new thread and send ping
  //Since, bionic and uClibc doesn't support C++ libs,
  //We have to use pthread instead of std::thread

  /*
  if(pthread_create(&this -> ping_pthread, NULL, this -> ping_thread, NULL)) {
    //Failed to create thread
    return -1;
  }
  */

  return 0;
}

//TODO: non-static member function cannot passed into pthread_create.
//Make it static. (get this from arg? maybe.)
void TCPing::ping_thread(void *arg) {
  //Private function - ping thread.
  //since send_packet is MT-Safe, there is no lock needed for
  //sending packet.

  //this -> ipsock -> send_packet() //TODO: send packet.
}
