#ifndef __TCPING_H_
#define __TCPING_H_

#include <net/if.h>
#include <pthread.h>

class TCPing {
 public:
  TCPing(int ipver, const char *ifname, const char *targetaddr, int port);
  ~TCPing();
  int init_ping();
  int start_ping();
  int stop_ping();
 private:
  static void* ping_thread(void *arg);
  IPsock *ipsock;
  pthread_t ping_pthread;
  bool isActive;
};

#endif
