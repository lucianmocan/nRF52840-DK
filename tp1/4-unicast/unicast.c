#include "contiki.h"
#include "dev/button-hal.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include <stdio.h>
#include <string.h>

#define BUFSIZE 256

// TODO1 création structure message
struct message {
    uint8_t seq_num;
    char text[10000];
};


/*---------------------------------------------------------------------------*/
PROCESS(unicast_process, "Unicast on Timeout");
AUTOSTART_PROCESSES(&unicast_process);
/*---------------------------------------------------------------------------*/

// fonction de réception des messages
void input_callback(const void *data, uint16_t len, const linkaddr_t *src,
                    const linkaddr_t *dst)
{

    // TODO6 afficher la source ainsi que les informations contenues dans le
    // message reçu
    struct message *msg = (struct message *)data;
    printf("Message reçu de ");
    for(int i = 0; i < LINKADDR_SIZE; i++) {
        printf("%02x", src->u8[i]);
        if(i < LINKADDR_SIZE - 1) printf(":");
    }
    printf(" - Texte: %s, Séquence: %d\n", msg->text, msg->seq_num);
}

PROCESS_THREAD(unicast_process, ev, data)
{

    static struct etimer timer_timer;
    static linkaddr_t dest_addr = {{0xf4, 0xce, 0x36, 0x5a, 0xb2, 0xbe, 0x02, 0x74}};
    // static linkaddr_t dest_addr = {{0xf4, 0xce, 0x36, 0x0e, 0xda, 0x64, 0xa5, 0x87}};

    PROCESS_BEGIN();

    etimer_set(&timer_timer, 5 * CLOCK_SECOND);


    // initialiser le callback de réception
    nullnet_set_input_callback(input_callback);


    // TODO2 créer le message à envoyer
    static struct message msg;
    msg.seq_num = 0;
    strcpy(msg.text, "so, WHO IS THEO!!");


    while (1) {

        // TODO3 attendre un événement de type "relâchement de bouton"
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_timer));

        // TODO4 vérifier que l'événement est déclenché par le bouton 1 (par
        // ex.)
        // button_hal_button_t *btn = (button_hal_button_t *)data;
        // if (btn->unique_id == 0) {

        // TODO5 positionnez les variables associées à la couche nullnet
        nullnet_buf = (uint8_t *)&msg;
        nullnet_len = strlen(msg.text) + 1 + sizeof(msg.seq_num);

        // envoi du message
        NETSTACK_NETWORK.output(&dest_addr);
        printf("Message unicast envoyé vers %02x:%02x : %s (%d)\n", 
            dest_addr.u8[0], dest_addr.u8[1], msg.text, msg.seq_num);

        msg.seq_num++;
        // }

        etimer_reset(&timer_timer);

    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
