#include "contiki.h"
#include "dev/button-hal.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include <stdio.h>
#include <string.h>

#define BUFSIZE 256

// TODO1 création structure message


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

}

PROCESS_THREAD(broadcast_button_process, ev, data)
{
    PROCESS_BEGIN();

    // initialiser le callback de réception
    nullnet_set_input_callback(input_callback);

    printf("Processus de diffusion démarré, en attente du bouton.\n");

    // TODO2 créer le message à envoyer


    while (1) {

        // TODO3 attendre un événement de type "relâchement de bouton"


        // TODO4 vérifier que l'événement est déclenché par le bouton 1 (par
        // ex.)

      
            // TODO5 positionnez les variables associées à la couche nullnet
            nullnet_buf = ???
            nullnet_len = ???

            // envoi du message
            NETSTACK_NETWORK.output(NULL);
            printf("Message envoyé : %s (%d)\n", ???, ???);
        }
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
