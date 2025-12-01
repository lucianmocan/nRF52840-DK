#include "contiki.h"
#include "net/routing/routing.h"
#include <stdlib.h>
#include <sys/log.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <etimer.h>

#define LOG_MODULE "6LoWPAN"
#ifndef LOG_CONF_LEVEL_6LOWPAN
#define LOG_LEVEL LOG_LEVEL_INFO
#else
#define LOG_LEVEL LOG_CONF_LEVEL_6LOWPAN
#endif


/*---------------------------------------------------------------------------*/
PROCESS(rpl_root_process, "RPL DoDAG Root Process");
AUTOSTART_PROCESSES(&rpl_root_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(rpl_root_process, ev, data) 
{
    static struct etimer et;
    int is_coordinator;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    is_coordinator = NETSTACK_ROUTING.node_is_root();

    if (!is_coordinator) {
        LOG_INFO("Setting up node as RPL DODAG root...\n");

        uip_ipaddr_t prefix;
        uip_ip6addr(&prefix, 0xfd05, 0, 0, 0, 0, 0, 0, 0);

        NETSTACK_ROUTING.root_set_prefix(&prefix, NULL);
        NETSTACK_ROUTING.root_start();

        LOG_INFO("Node is now RPL DODAG root\n");
        LOG_INFO_6ADDR(&prefix);
        LOG_INFO_("\n");
    } else {
        LOG_INFO("Node is already RPL DODAG root\n");
    }
   
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
