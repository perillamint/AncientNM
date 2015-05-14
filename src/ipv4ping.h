#ifndef __IPV4SOCK_H_
#define __IPV4SOCK_H_

#include <net/if.h>

typedef struct {
  //Member func
  int (*open_sock)();
  int (*close_sock)();
  int (*send_packet)();
  int (*recv_packet)();
  
  //Private stuff
  int soc
