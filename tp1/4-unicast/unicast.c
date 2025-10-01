#include "contiki.h"
#include "dev/button-hal.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include <stdio.h>
#include <string.h>
#include <sys/log.h>

#define BUFSIZE 256

#define LOG_MODULE "Unicast"
#ifndef LOG_CONF_LEVEL_UNICAST
#define LOG_LEVEL LOG_LEVEL_INFO
#else
#define LOG_LEVEL LOG_CONF_LEVEL_UNICAST
#endif


// create the message struct
struct message {
    uint8_t seq_num;
    char text[BUFSIZE];
} __attribute__((packed)); // no padding so seq_num is always 1 byte


/*---------------------------------------------------------------------------*/
PROCESS(unicast_process, "Unicast on Timeout");
AUTOSTART_PROCESSES(&unicast_process);
/*---------------------------------------------------------------------------*/

// receive callback function
void input_callback(const void *data, uint16_t len, const linkaddr_t *src,
                    const linkaddr_t *dst)
{

    // display the source and the message
    struct message *msg = (struct message *)data;
    LOG_INFO("Message received: ");
    LOG_INFO_LLADDR(src);
    LOG_INFO_(" - Text: %s, Sequence: %d\n", msg->text, msg->seq_num);
}

PROCESS_THREAD(unicast_process, ev, data)
{

    static struct etimer timer_timer;
    static linkaddr_t dest_addr = {{0xf4, 0xce, 0x36, 0x1d, 0xcc, 0x8f, 0xd3, 0x96}};

    PROCESS_BEGIN();

    etimer_set(&timer_timer, 5 * CLOCK_SECOND);


    // initialize the receive callback
    nullnet_set_input_callback(input_callback);


    // create the message to send
    static struct message msg = {0};
    msg.seq_num = 0;
    strcpy(msg.text, "so, WHO IS THEO!!");


    while (1) {

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_timer));

        nullnet_buf = (uint8_t *)&msg;
        nullnet_len = 1 + strlen(msg.text) + 1; // seq num + text length + null terminator

        // send message to destination
        NETSTACK_NETWORK.output(&dest_addr);
        LOG_INFO_("Message sent to: ");
        LOG_INFO_LLADDR(&dest_addr);
        LOG_INFO_(" - Text: %s, Sequence: %d\n", msg.text, msg.seq_num);

        msg.seq_num++;

        etimer_reset(&timer_timer);
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
