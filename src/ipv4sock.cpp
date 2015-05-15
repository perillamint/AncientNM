#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ipv4sock.h"

IPV4sock::IPV4sock(char *ifname) { 
  this -> ifreq = malloc(sizeof(struct ifreq));
  this -> header_len = sizeof(struct iphdr));
  this -> packet_header = malloc(this -> header_len);

  memset(this -> packet_header, 0, this -> header_len);
  memset(this -> ifreq, 0, sizeof(struct ifreq));
  
  memcpy(this -> ifreq -> ifr_name, ifname, IFNAMESZ);
}

IPV4sock::~IPV4sock() {
  if(this -> sockfd > -1) 

int IPV4sock::open_socket() {
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

  this -> sockfd = sockfd;
  
  return 0;
}

int IPV4sock::close_socket() {
  close(this -> sockfd);
  this -> sockfd = -1;

  return 0;
}

 int IPV4sock::send_packet(uint8_t proto, uint8_t ttl, char* destaddr, uint16_t destport, void *pktbody, int pktbodysize) {
  void *pkt = malloc(this -> header_len + pktbodysize);
  struct sockaddr_in sockaddr;

  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(destport);
  sockaddr.sin_addr.s_addr = this -> iphdr -> saddr;
  
  this -> iphdr -> protocol = proto;
  this -> iphdr -> ttl = ttl;
  this -> iphdr -> daddr = inet_addr(destaddr);
  this -> iphdr -> tot_len = (this -> header_len + pktbodysize) / 4;
  //TODO: generate packet id.

  //Generate checksum
  this -> iphdr -> check = 0;

  uint16_t *header = (uint16_t*)this -> iphdr;

  uint32_t sum = 0;

  for(int i = 0; i < this -> header_len * 2; i++) {
    sum += *(header + i);
  }

  uint8_t chk = (sum & 0xF0000)  >> 16;
  sum += chk;

  sum = !sum & 0xFFFF;

  this -> iphdr -> check = (uint16_t)sum;

  memcpy(pkt, this -> iphdr, this -> header_len);
  if(pktbodysize > 0) {
    memcpy(pkt + this -> header_len, pktbody, pktbodysize);
  }
  
  sendto(this -> sockfd, pkt, this -> header_len + pktbodysize, 0, &sockaddr, sizeof(sockaddr));
  return 0;
}
