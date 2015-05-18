#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <sys/ioctl.h>
#include <net/if.h>

#include "ipv4sock.h"

IPV4sock::IPV4sock(const char *ifname) { 
  this -> ifreq = (struct ifreq*)malloc(sizeof(struct ifreq));
  this -> header_len = sizeof(struct iphdr);
  this -> iphdr = (struct iphdr*)malloc(this -> header_len);
  this -> sockfd = -1;

  memset(this -> iphdr, 0, this -> header_len);
  memset(this -> ifreq, 0, sizeof(struct ifreq));
  
  memcpy(this -> ifreq -> ifr_name, ifname, IFNAMSIZ);
}

IPV4sock::~IPV4sock() {
  if(this -> sockfd > -1)
    close(this -> sockfd);

  free(this -> iphdr);
  free(this -> ifreq);
}

int IPV4sock::open_socket(const char *destaddr) {
  const static int yes = 1;
  int sockfd;
  
  if(this -> sockfd != -1) {
    return -1;
  }
  
  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  
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
  if(ioctl(sockfd, SIOCGIFADDR, this -> ifreq) < 0)
  {
    //Uh-oh. ioctl failed.
    close(sockfd);
    return -1;
  }

  /* fields to fill
   * IP version (version)
   * Header length (ihl) //32-bit word
   * protocol (protocol) *
   * total length (tot_len) -
   * packet ID (id) -
   * TTL (ttl) *
   * checksum (checksum) -
   * Source addr (saddr)
   * Dest addr (daddr) *
   *
   * *: Need to fill when sending packet
   * -: Need to calculate when sending packet
   */
  
  this -> iphdr -> version = 4; //IPv4
  this -> iphdr -> ihl = this -> header_len / 4;
  this -> iphdr -> saddr = ((struct sockaddr_in *)&this -> ifreq
			    -> ifr_addr) -> sin_addr.s_addr;
  this -> iphdr -> daddr = inet_addr(destaddr);

  this -> sockfd = sockfd;
  
  return 0;
}

int IPV4sock::close_socket() {
  if(this -> sockfd >= 0) {
    close(this -> sockfd);
  }
  
  this -> sockfd = -1;

  return 0;
}

//TODO: BUG HERE. NEED TO DEBUG
int IPV4sock::getipaddr(char *addr) {
  if(this -> sockfd < 0) {
    return -1;
  }
  
  if(ioctl(this -> sockfd, SIOCGIFADDR, this -> ifreq) < 0)
  {
    //Uh-oh. ioctl failed.
    return -1;
  }
  
  inet_ntop(AF_INET, (void*)&((struct sockaddr_in *)&this -> ifreq
		      -> ifr_addr) -> sin_addr,
	    addr, sizeof(struct in_addr));

  return 0;
}

int IPV4sock::send_packet(uint8_t proto, uint8_t ttl, uint16_t destport, const void *pktbody, int pktbodysize) {
  void *pkt = malloc(this -> header_len + pktbodysize);
  struct sockaddr_in sockaddr;

  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(destport);
  sockaddr.sin_addr.s_addr = this -> iphdr -> saddr;
  
  this -> iphdr -> protocol = proto;
  this -> iphdr -> ttl = ttl;
  this -> iphdr -> tot_len = (this -> header_len + pktbodysize) / 4;
  //TODO: generate packet id.

  //Generate checksum
  this -> iphdr -> check = 0;

  uint16_t *header = (uint16_t*)this -> iphdr;

  uint32_t sum = 0;

  for(int i = 0; i < this -> header_len / 2; i++) {
    sum += ntohs(*(header + i));
  }

  uint16_t chk = (sum & 0xFFFF0000) >> 16;
  sum += chk;

  sum = ~sum & 0xFFFF;

  this -> iphdr -> check = htons((uint16_t)sum);

  memcpy(pkt, this -> iphdr, this -> header_len);
  if(pktbodysize > 0) {
    memcpy((char*)pkt + this -> header_len, pktbody, pktbodysize);
  }

  printf("sendto!");
  int foo = sendto(this -> sockfd, pkt, this -> header_len + pktbodysize, 0, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

  printf("%d\n", foo);
  return 0;
}
