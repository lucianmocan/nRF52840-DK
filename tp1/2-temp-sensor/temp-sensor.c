#include "../arch/platform/nrf52840/common/temperature-sensor.h"
#include "contiki.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(read_temp_process, "Read Temperature Process");
AUTOSTART_PROCESSES(&read_temp_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(read_temp_process, ev, data)
{
    // TODO1 déclaration du temporisateur


    PROCESS_BEGIN();

    // TODO1 armement du temporisateur à 5 sec.


    // TODO2 activer le capteur


    while (1) {

        // TODO1 attente passive jusqu'à l'expiration du temporisateur
        // puis affichage d'un message


        // TODO2 récupérer puis afficher la valeur brute du capteur


        // TODO3 conversion puis affichage de la température


        // TODO1 réarmez le temporisateur pour le cycle suivant

    }

    // TODO2 désactivez le capteur


    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
