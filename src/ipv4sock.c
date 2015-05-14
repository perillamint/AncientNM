#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

IPV4sock *new_IPV4sock(char *ifname)
{
  IPV4sock *sock = malloc(sizeof (IPV4sock));
  
