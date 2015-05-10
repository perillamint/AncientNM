#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <config.h>
#include "tcping.h"

/**
 * It uses raw tcp socket to ping servers under firewall.
 */
