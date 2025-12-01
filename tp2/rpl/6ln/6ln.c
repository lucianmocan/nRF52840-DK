#include "contiki.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include <string.h>
#include <stdio.h>
#include "sys/log.h"
#include <stdint.h>

#define LOG_MODULE "6LN-Receiver"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_PORT 4321
#define BUFSIZE 64

struct message {
    uint8_t seq_num;
    uint16_t temperature;
    char text[BUFSIZE];
} __attribute__((packed));

/*---------------------------------------------------------------------------*/
PROCESS(udp_receiver, "Receiver UDP");
AUTOSTART_PROCESSES(&udp_receiver);
/*---------------------------------------------------------------------------*/
// Callback for received UDP packets
static void
udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen)
{
    struct message *msg = (struct message *)data;
    LOG_INFO("Received from ");
    LOG_INFO_6ADDR(sender_addr);
    LOG_INFO_(": seq %d, temp %d, msg '%s'\n", msg->seq_num, msg->temperature, msg->text);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_receiver, ev, data)
{
    static struct simple_udp_connection udp_conn;

    PROCESS_BEGIN();

    // This node will join an existing RPL network
    NETSTACK_ROUTING.root_start();

    // Register the UDP connection
    simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);
    LOG_INFO("UDP receiver started. Listening on port %u\n", UDP_PORT);

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
