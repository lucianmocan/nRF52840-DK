#include "../arch/platform/nrf52840/common/temperature-sensor.h"
#include "contiki.h"
#include <stdio.h>
#include <etimer.h>

/*---------------------------------------------------------------------------*/
PROCESS(read_temp_process, "Read Temperature Process");
AUTOSTART_PROCESSES(&read_temp_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(read_temp_process, ev, data)
{
    static struct etimer timer_timer;

    PROCESS_BEGIN();

    // set timer at 5 seconds
    etimer_set(&timer_timer, 5 * CLOCK_SECOND);


    SENSORS_ACTIVATE(temperature_sensor);

    while (1) {

        // passive wait until the timer expires
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_timer));
        // then display a message
        printf("Timer expired, reading temperature...\n");

        // retrieve and display the raw value from the sensor
        int raw_value = temperature_sensor.value(0);
        printf("Raw temperature value: %d\n", raw_value);

        // convert then display the temperature in Celsius
        int temp_int = raw_value / 4;
        int temp_frac = (raw_value % 4) * 25;  // Convert remainder to hundredths

        // Handle negative temperatures properly
        if (raw_value < 0) {
            temp_frac = -temp_frac;
        }
        
        printf("Temperature: %d.%d C\n", temp_int, temp_frac);

        // reschedule the timer
        etimer_reset(&timer_timer);

    }

    // deactivate the sensor
    SENSORS_DEACTIVATE(temperature_sensor);


    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
