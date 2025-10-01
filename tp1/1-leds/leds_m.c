#include "contiki.h"
#include "dev/button-hal.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include <stdio.h>


/*---------------------------------------------------------------------------*/
PROCESS(leds_m_process, "Leds m process");
AUTOSTART_PROCESSES(&leds_m_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(leds_m_process, ev, data)
{

    PROCESS_BEGIN();

    gpio_hal_arch_init();
    gpio_hal_arch_pin_set_output(0, 13); // LED1
    gpio_hal_arch_pin_set_output(0, 14); // LED2
    gpio_hal_arch_pin_set_output(0, 15); // LED3
    gpio_hal_arch_pin_set_output(0, 16); // LED4

    while (1) {

        PROCESS_YIELD();

        // l'événement est-il un bouton relaché ?
        if (ev == button_hal_release_event) {
            button_hal_button_t *button = (button_hal_button_t *)data;

            switch(button->unique_id) {
                case 0:
                    gpio_hal_arch_toggle_pin(0, 13); // Toggle LED1
                    break;
                case 1:
                    gpio_hal_arch_toggle_pin(0, 14); // Toggle LED2
                    break;
                case 2:
                    gpio_hal_arch_toggle_pin(0, 15); // Toggle LED3
                    break;
                case 3:
                    gpio_hal_arch_toggle_pin(0, 16); // Toggle LED4
                    break;
            }
        }
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
