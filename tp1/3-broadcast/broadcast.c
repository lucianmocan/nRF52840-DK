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
PROCESS(broadcast_button_process, "Broadcast on Button Release");
AUTOSTART_PROCESSES(&broadcast_button_process);
/*---------------------------------------------------------------------------*/

// fonction de réception des messages
void input_callback(const void *data, uint16_t len, const linkaddr_t *src,
                    const linkaddr_t *dst)
{

    // TODO6 afficher la source ainsi que les informations contenues dans le
    // message reçu
    struct message *msg = (struct message *)data;
    printf("Message reçu de %02x:%02x - Texte: %s, Séquence: %d\n",
           src->u8[0], src->u8[1], 
           msg->text, msg->seq_num);
}

PROCESS_THREAD(broadcast_button_process, ev, data)
{
    PROCESS_BEGIN();

    // initialiser le callback de réception
    nullnet_set_input_callback(input_callback);

    printf("Processus de diffusion démarré, en attente du bouton.\n");

    // TODO2 créer le message à envoyer
    static struct message msg;
    msg.seq_num = 0;
    strcpy(msg.text, "so, WHO IS GAY!!");


    while (1) {

        // TODO3 attendre un événement de type "relâchement de bouton"
        PROCESS_WAIT_EVENT_UNTIL(ev == button_hal_release_event);

        // TODO4 vérifier que l'événement est déclenché par le bouton 1 (par
        // ex.)
        button_hal_button_t *btn = (button_hal_button_t *)data;
        if (btn->unique_id == 0) {

            // TODO5 positionnez les variables associées à la couche nullnet
            nullnet_buf = (uint8_t *)&msg;
            nullnet_len = strlen(msg.text) + 1 + sizeof(msg.seq_num);
    
            // envoi du message
            NETSTACK_NETWORK.output(NULL);
            printf("Message envoyé : %s (%d)\n", msg.text, msg.seq_num);

            msg.seq_num++;
        }

    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
