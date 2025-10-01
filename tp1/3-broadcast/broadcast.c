#include "contiki.h"
#include "dev/button-hal.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include <stdio.h>
#include <string.h>
#include <sys/log.h>

#define BUFSIZE 256

#define LOG_MODULE "Broadcast"
#define LOG_LEVEL LOG_LEVEL_INFO

// create the message structure
struct message {
    uint8_t seq_num;
    char text[BUFSIZE];
} __attribute__((packed));    // Ensure no padding;


/*---------------------------------------------------------------------------*/
PROCESS(broadcast_button_process, "Broadcast on Button Release");
AUTOSTART_PROCESSES(&broadcast_button_process);
/*---------------------------------------------------------------------------*/

// message reception function
void input_callback(const void *data, uint16_t len, const linkaddr_t *src,
                    const linkaddr_t *dst)
{

    // display the source and the content of the received message
    struct message *msg = (struct message *)data;
    LOG_INFO("Message received: ");
    LOG_INFO_LLADDR(src);
    LOG_INFO_(" - Text: %s, Sequence: %d\n", msg->text, msg->seq_num);
}
PROCESS_THREAD(broadcast_button_process, ev, data)
{
    PROCESS_BEGIN();

    // initialize the reception callback
    nullnet_set_input_callback(input_callback);

    LOG_INFO("Broadcast process started, waiting for button.\n");

    // create the message to send
    static struct message msg = {0};
    msg.seq_num = 0;
    strcpy(msg.text, "MEGA KNIGHT!");


    while (1) {

        // wait for a button release event
        PROCESS_WAIT_EVENT_UNTIL(ev == button_hal_release_event);

        // check that the event was triggered by button 1
        button_hal_button_t *btn = (button_hal_button_t *)data;
        if (btn->unique_id == BUTTON_HAL_ID_BUTTON_ZERO) {

            // set the variables associated with the nullnet layer
            nullnet_buf = (uint8_t *)&msg;
            nullnet_len = 1 + strlen(msg.text) + 1;  // seq_num + text length + null terminator

            // send the message
            NETSTACK_NETWORK.output(NULL);
            LOG_INFO_("Message sent: %s (%d)\n", msg.text, msg.seq_num);

            msg.seq_num++;
        }

    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
