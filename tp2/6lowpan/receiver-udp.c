#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include <string.h>
#include <stdio.h>
#include "sys/log.h"
#include "project-conf.h"

#include <stdint.h>
#include <sys/log.h>

#define BUFSIZE 256


#define LOG_MODULE "6LoWPAN"
#ifndef LOG_CONF_LEVEL_6LOWPAN
#define LOG_LEVEL LOG_LEVEL_INFO
#else
#define LOG_LEVEL LOG_CONF_LEVEL_6LOWPAN
#endif


#define PORT_SENDER 61616
#define PORT_RECV 61617


/*---------------------------------------------------------------------------*/
PROCESS(udp_receiver, "Receiver UDP");
AUTOSTART_PROCESSES(&udp_receiver);
/*---------------------------------------------------------------------------*/
struct message {
    uint8_t seq_num;
    uint16_t temperature;
    char text[BUFSIZE];
} __attribute__((packed)); 

// fonction de réception des messages
void udp_rx_callback(struct simple_udp_connection *udp_con,
					 const uip_ipaddr_t *src,
					 uint16_t sport,
					 const uip_ipaddr_t *dest,
					 uint16_t dport,
					 const uint8_t *data,
					 uint16_t size)
{
	struct message *mess = (struct message *)data;
	LOG_INFO("Received %u bytes from ", size);
	LOG_INFO_6ADDR(src);
	LOG_INFO_(", port %u -> ", sport);
	LOG_INFO_6ADDR(dest);
	LOG_INFO_(", port %u: '", dport);

	LOG_INFO_("%d, %d, %s", mess->seq_num, mess->temperature, mess->text);

	LOG_INFO_("'\n");
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_receiver, ev, data)
{
	PROCESS_BEGIN();
	static struct simple_udp_connection udp_conn;
	simple_udp_register(&udp_conn, PORT_RECV, NULL, PORT_SENDER, udp_rx_callback);
	while (1)
	{
		PROCESS_YIELD();
	}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
