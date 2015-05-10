/**
 * It uses raw tcp socket to ping servers under firewall.
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/ip.h>
#include <linux/tcp.h>

#include <sys/ioctl.h>
#include <net/if.h>

#include <config.h>
#include "tcping.h"

Tcping *new_Tcping(char *ifname)
{
  Tcping *tcping = malloc(sizeof (tcping));
  memset(tcping, 0, sizeof (tcping));
  
  tcping -> init_ping = init_ping;

  tcping -> _sockfd = -1;
  tcping -> _header_len = sizeof(struct tcphdr) + sizeof(struct iphdr);
  tcping -> _header = malloc(tcping -> _header_len);
  tcping -> _ifreq = malloc(sizeof(struct ifreq));

  memset(tcping -> _header, 0, tcping -> _header_len);
  memset(tcping -> _ifreq, 0, sizeof(struct ifreq));
  
  memcpy(tcping -> _ifreq -> ifr_name, ifname, IFNAMSIZ);

  return tcping;
}

void *delete_Tcping(Tcping *this)
{
  if(this -> _header != NULL)
    free(this -> _header);
  
  if(this -> _ifreq != NULL)
    free(this -> _ifreq);
  
  if(this -> _sockfd != -1)
    close(this -> _sockfd);

  free(this);
}

inline struct iphdr* get_iphdr(Tcping *this)
{
  return this -> _header;
}

inline struct tcphdr* get_tcphdr(Tcping *this)
{
  return this -> _header + sizeof(struct iphdr);
}

static int init_ping(Tcping *this)
{
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  const static int yes = 1;

  if(sockfd < 0)
    return -1; //It fails

  //Use IP socket and do not automatically calculate IP header.
  //We have to make header to send stealth SYN ping.
  if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &yes, sizeof(yes)) < 0)
  {
    //Uh-oh. Something goes wrong!
    close(sockfd);
    return -1;
  }

  //Get IP address using ioctl.
  if(ioctl(sockfd, SIOCGIFADDR, this -> _ifreq) < 0)
  {
    //Uh-oh. ioctl failed.
    close(sockfd);
    return -1;
  }

  //Now, this -> _ifreq has a IP addr.
  //It is in_addr_t so we can copy it into header directly.
  struct iphdr *iphdr = get_iphdr(this);

  iphdr -> saddr = ((struct sockaddr_in *)&this -> _ifreq -> ifr_addr)
		    -> sin_addr.s_addr;

  //TODO: iphdr -> daddr from constructor

  return 0;
}
