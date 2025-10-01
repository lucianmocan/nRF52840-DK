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
    static struct etimer timer_timer;


    PROCESS_BEGIN();

    // TODO1 armement du temporisateur à 5 sec.
    etimer_set(&timer_timer, 5 * CLOCK_SECOND);


    // TODO2 activer le capteur
    SENSORS_ACTIVATE(temperature_sensor);

    while (1) {

        // TODO1 attente passive jusqu'à l'expiration du temporisateur
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_timer));
        // puis affichage d'un message
        printf("Timer expired, reading temperature...\n");

        // TODO2 récupérer puis afficher la valeur brute du capteur
        int raw_value = temperature_sensor.value(0);
        printf("Raw temperature value: %d\n", raw_value);

        // TODO3 conversion puis affichage de la température
        // Raw value is in units of 0.1°C, so divide by 10
        int temp_int = raw_value / 4;
        int temp_frac = (raw_value % 4) * 25;  // Convert remainder to hundredths
        
        // Handle negative temperatures properly
        if (raw_value < 0) {
            temp_frac = -temp_frac;
        }
        
        printf("Temperature: %d.%d C\n", temp_int, temp_frac);

        // TODO1 réarmez le temporisateur pour le cycle suivant
        etimer_reset(&timer_timer);

    }

    // TODO2 désactivez le capteur
    SENSORS_DEACTIVATE(temperature_sensor);


    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
