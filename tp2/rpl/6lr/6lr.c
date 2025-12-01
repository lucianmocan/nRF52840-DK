// dest_addr, etimer and seq_num declarations before process begin
#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include <string.h>
#include <stdio.h>
#include "sys/log.h"
#include "../arch/platform/nrf52840/common/temperature-sensor.h"
#include "sys/etimer.h"
#include <stdint.h>
#include <stddef.h>
#include "lib/random.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/multicast/uip-mcast6.h"
#include "net/ipv6/uip-packetqueue.h"

#define LOG_MODULE "6LR-Sender"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_PORT 4321
#define SEND_INTERVAL (10 * CLOCK_SECOND)
#define BUFSIZE 64

struct message {
    uint8_t seq_num;
    uint16_t temperature;
    char text[BUFSIZE];
} __attribute__((packed));

/*---------------------------------------------------------------------------*/
PROCESS(udp_sender, "Sender UDP");
AUTOSTART_PROCESSES(&udp_sender);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_sender, ev, data)
{
    static struct simple_udp_connection udp_conn;
    static struct etimer periodic_timer;
    static struct message msg;
    static uip_ipaddr_t dest_ipaddr;

    PROCESS_BEGIN();

    // This node will join an existing RPL network
    NETSTACK_ROUTING.root_start();

    // Register the UDP connection
    simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);
    
    etimer_set(&periodic_timer, SEND_INTERVAL);
    SENSORS_ACTIVATE(temperature_sensor);

    msg.seq_num = 0;
    strcpy(msg.text, "Hello from 6LR");

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        // Check for RPL convergence
        if(NETSTACK_ROUTING.node_is_reachable() && uip_ds6_get_global(ADDR_PREFERRED) != NULL) {
            // Set the 6LN's GLOBAL IP address here.
            // Find this address from the 6LN's log output (it will start with fd05::)
            // Example: uip_ip6addr(&dest_ipaddr, 0xfd05, 0, 0, 0, 0xabcd, 0xef, 0x1234, 0x5678);
            // uip_ip6addr(&dest_ipaddr, 0xfd05, 0, 0, 0, 0, 0, 0, 2); // <-- REPLACE THIS with 6LN's real address

            msg.temperature = temperature_sensor.value(0);
            LOG_INFO("Sending to ");
            LOG_INFO_6ADDR(&dest_ipaddr);
            LOG_INFO_(": seq %d, temp %d\n", msg.seq_num, msg.temperature);

            simple_udp_sendto(&udp_conn, &msg, sizeof(msg), &dest_ipaddr);
            msg.seq_num++;
        } else {
            LOG_INFO("RPL not converged yet, waiting...\n");
        }
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
