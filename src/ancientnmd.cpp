#include <stdio.h>
#include <unistd.h>

#include "ipsock.h"
#include "tcping.h"

int main(int argc, char **argv) {
  printf("Hello, world!");
  
  TCPing *tcping = new TCPing(4, "wlp3s0", "", 80);
  
  tcping -> init_ping();
  tcping -> start_ping();
  
  delete tcping;
  return 0;
}
