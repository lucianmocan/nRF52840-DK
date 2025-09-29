#include "contiki.h"
#include "dev/button-hal.h"
#include "dev/gpio-hal.h"

/*---------------------------------------------------------------------------*/
PROCESS(leds_m_process, "Leds m process");
AUTOSTART_PROCESSES(&leds_m_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(leds_m_process, ev, data)
{

    PROCESS_BEGIN();

    while (1) {

        PROCESS_YIELD();

        // TODO1
        // l'événement est-il un bouton relaché ?


        // TODO2
        // si oui, il faut identifier le bouton qui a été relaché


        // TODO3
        // il faut allumer ou éteindre la led correspondante au bouton

    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
