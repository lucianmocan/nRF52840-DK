#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include <string.h>
#include <stdio.h>
#include "sys/log.h"

#define PORT_SENDER XXXX
#define PORT_RECV XXXX


/*---------------------------------------------------------------------------*/
PROCESS(udp_receiver, "Receiver UDP");
AUTOSTART_PROCESSES(&udp_receiver);
/*---------------------------------------------------------------------------*/
// fonction de réception des messages
void udp_rx_callback(struct simple_udp_connection *udp_con,
		     const uip_ipaddr_t *src,
		     uint16_t sport,
		     const uip_ipaddr_t *dest,
		     uint16_t dport,
		     const uint8_t *data,
		     uint16_t size)
{
	
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_receiver, ev, data)
{
	PROCESS_BEGIN();

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
