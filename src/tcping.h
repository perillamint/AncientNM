#ifndef __TCPING_H_
#define __TCPING_H_

#include <net/if.h>

typedef struct {
  //Member func
  int (*init_ping)();
  int (*send_ping)();

  //Member vars.

  //Private stuff - DO NOT MANIPULATE THIS DIRECTLY.
  //Private member.
  int _sockfd;
  int _header_len;
  void *_header;
  struct ifreq *_ifreq;
  char _pingtarg[40];
} Tcping;

Tcping *new_Tcping();
static int init_ping(Tcping *this);

#endif
