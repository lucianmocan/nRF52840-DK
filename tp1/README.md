# Internet of Things

## TP 1: Getting started with Contiki-NG and the development environment

In this TP, you will explore embedded systems programming using Contiki-NG, a lightweight operating system for connected objects.  
You will use an nRF52840 DK development board, which includes LEDs, buttons, a temperature sensor, and a radio interface operating in the 2.4 GHz band.  
This TP will help you understand how to interact with hardware through simple programs, and how to set up communication mechanisms in wireless networks.  
You will also learn how to use timers and logs for efficient application management.

---

## 1. Managing LEDs and buttons

Complete the program located in `tp1/1-leds/leds_m.c` so that each button controls the on/off state of an LED (for example, button $x_i$ should turn LED $y_j$ on or off).

- Reminder on `PROCESS_YIELD()`: This macro explicitly yields control to the scheduler and suspends the process without any condition.  
If an event occurs, the process is rescheduled.

- Event verification: To detect when a button is released (TODO1 in the code), check if the variable `ev` matches the button-release event.  
See the [button documentation](https://docs.contiki-ng.org/en/develop/_api/group__button__hal.html) to find the variable associated with this event.

- Identifying the button: Next, identify which button generated the event (TODO2 in the code).  
The variable `data` will be a pointer to a variable of type `button_hal_button_t` (defined as `struct button_hal_button_s`).  
Check the [source code](https://docs.contiki-ng.org/en/develop/_api/button-hal_8h_source.html) to locate the field in this structure that identifies a button.

- Controlling the associated LED: Finally, switch on or off the LED corresponding to the released button (TODO3 in the code).  
To do this, write to the GPIO assigned to the LED (port + pin). See the [Contiki-NG documentation](https://docs.contiki-ng.org/en/develop/_api/group__gpio-hal.html) for the appropriate function.  
The port and pin numbers for each LED and button are listed in the [board documentation](https://docs.nordicsemi.com/bundle/ug_nrf52840_dk/page/UG/dk/hw_buttons_leds.html).

---

## 2. Managing the temperature sensor

The nRF52840 DK boards include an internal sensor that measures the board’s temperature.  
In this exercise, you will complete the program in `tp1/2-temp-sensor/temp-sensor.c` to display every 5 seconds both the raw sensor value and the temperature in degrees Celsius.

- Timer management: Create a timer of type `etimer` and configure it to trigger every 5 seconds. In the main loop, suspend the process until the timer expires, then print a message with the temperature. Finally, reset the timer for the next cycle (see TODO1 in the source code). See the [Contiki-NG timer documentation](https://docs.contiki-ng.org/en/master/doc/programming/Timers.html) for help.

- Sensor activation and reading: Activate the temperature sensor and fetch its raw value each time the timer expires. Print this value. Finally, deactivate the sensor before the process terminates (see TODO2 in the source code). For more details, check `arch/platform/nrf52840/common/temperature-sensor.h`, `arch/platform/nrf52840/common/temperature-sensor.c`, and `os/lib/sensors.h`.

- Converting raw value to Celsius: According to the [board documentation](https://docs.nordicsemi.com/bundle/ps_nrf52840/page/temp.html), the sensor produces a fixed-point raw measurement where each increment represents 0.25 °C. Convert the raw value to Celsius and display the result (see TODO3 in the source code).

---

## 3. Broadcast communication

Complete the program in `tp1/3-broadcast/broadcast.c` so that each time button 1 is released, the board sends a broadcast message containing `Hello, world` along with an 8-bit sequence number.

- Message construction: Create a `struct message` containing an 8-bit sequence number and a character array of `BUFSIZE` bytes (see TODO1 in the code).  
Create a variable of this type, set the sequence number and the string to send (see TODO2 in the code).

- Waiting for the event: Suspend the process until the “button released” event occurs using `PROCESS_WAIT_EVENT_UNTIL()` (see TODO3 in the source code).

- Identifying the button: Identify the button that triggered the event as you did in exercise 1 (see TODO4 in the code).

- Using _nullnet_: _nullnet_ is a network layer that does nothing, allowing applications to create and send messages directly to the MAC layer.  
To send your message with _nullnet_, copy the address of your `struct message` variable into the `nullnet_buf` pointer and set the message size in `nullnet_len` (TODO5 in the code).  
The message is sent with the macro `NETSTACK_NETWORK.output(NULL)` (`NULL` means broadcast).  
Also update the `printf` display and don’t forget to increment the sequence number.

- Receiving messages: When a message is received, the `input_callback()` function is called.  
Complete this function to display the message source and its contents (see TODO6 in the code).  
You can use `%02x` formatting to display the sender’s layer-2 address. See the [Contiki-NG documentation](https://docs.contiki-ng.org/en/master/_api/linkaddr_8h_source.html) for the address format.

---

## 4. Unicast communication

Complete the program in `tp1/4-unicast/unicast.c` so that your board sends a unicast message every 5 seconds.  
You can reuse code you developed:
- in exercise 2 for timer management (to send the message periodically),  
- in exercise 3 for sending a message with _nullnet_.  

The message should include a phrase like “Hello, world!” and a sequence number.  
The destination address must be that of one of your neighbor boards.

---

## 5. Managing logs

Contiki-NG uses a modular and configurable logging system to display diagnostic and debugging messages.  
The logging system mainly relies on the `LOG` macro, with several log levels that allow fine-grained control of displayed information:

- `LOG_LEVEL_NONE`: no message is logged.  
- `LOG_LEVEL_ERR`: for error messages.  
- `LOG_LEVEL_WARN`: for warnings.  
- `LOG_LEVEL_INFO`: for general information.  
- `LOG_LEVEL_DBG`: for detailed debugging messages.  

Log levels are configured per module, allowing precise control of the amount of information displayed for each component.  
For example, one module can be set to `LOG_LEVEL_DBG` for detailed output, while another remains at `LOG_LEVEL_ERR` to show only errors.  
Each message is printed through the serial output by default and includes metadata such as the source module and log level.

To add logs to your program:
- include `sys/log.h`  
- define the constants `LOG_LEVEL` (the log level to display) and `LOG_MODULE` (module name shown in logs).  

In the previous exercise’s code, replace `printf()` calls with the logging system at different levels (`LOG_ERR`, `LOG_INFO`, `LOG_DBG`, etc.).  
Experiment by changing `LOG_LEVEL` to observe the effects on displayed messages.

Then, modify how you define the log level so it can be set in a `project-conf.h` file, as is done for other existing logs.  
You can take inspiration from `sys/log-conf.h`.

---

## 6. Traffic capture and analysis

In this exercise, you will capture and analyze the network traffic generated by the nRF52840 boards.  
Work in pairs with the person who owns the board that sent messages in exercises 4 and 5. You will then have two boards: one acting as the sender, the other as the sniffer.

- Preparing the boards: On the first board (sender), run the program from the previous exercise (unicast with logs).  
On the second board (sniffer), run the `sensniff` program available in `tp1/6-sensniff`. This program allows the board to capture network frames and send them in real time to your machine.

- Configuration and capture with `tshark`: Connect the sniffer board to your computer and start a capture with `tshark` to record the exchanged IEEE 802.15.4 frames. See the [documentation](https://github.com/g-oikonomou/sensniff) for instructions on linking `sensniff` with `tshark`.

- Analyzing frame headers and content: Observe the layer-2 header fields such as the sequence number and frame types. What do you notice? Examine the contents of exchanged messages. The data should include the message “Hello, world!” with a sequence number. Check whether the messages are complete and correctly decoded.
