// dest_addr, etimer and seq_num declarations before process begin
#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include <string.h>
#include <stdio.h>
#include "sys/log.h"
#include "../arch/platform/nrf52840/common/temperature-sensor.h"
#include "simple-udp.h"
#include <etimer.h>
#include <stdint.h>

#define LOG_MODULE "6LoWPAN"
#ifndef LOG_CONF_LEVEL_6LOWPAN
#define LOG_LEVEL LOG_LEVEL_INFO
#else
#define LOG_LEVEL LOG_CONF_LEVEL_6LOWPAN
#endif

#define PORT_SENDER 8765
#define PORT_RECV 4321

#define BUFSIZE 64  // Reduce size - 256 is too large for typical UDP packets

struct message {
    uint8_t seq_num;
    uint16_t temperature;
    char text[BUFSIZE];
} __attribute__((packed)); 

static uip_ipaddr_t dest_addr;

static void 
udp_rx_callback(struct simple_udp_connection *udp_con,
         const uip_ipaddr_t *src,
         uint16_t sport,
         const uip_ipaddr_t *dest,
         uint16_t dport,
         const uint8_t *data,
         uint16_t size)
{
  // printf("Received response '%.*s' from ", size, (char *)data);
  // uip_debug_ipaddr_print(src);
  // printf("\n");
}

/*---------------------------------------------------------------------------*/
PROCESS(udp_sender, "Sender UDP");
AUTOSTART_PROCESSES(&udp_sender);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_sender, ev, data)
{
  static struct simple_udp_connection udp_conn;
  static struct etimer timer_timer;
  static struct message msg;  // Move to static

  PROCESS_BEGIN();

  etimer_set(&timer_timer, 5 * CLOCK_SECOND);

  simple_udp_register(&udp_conn, PORT_SENDER,
                      NULL, PORT_RECV,
                      udp_rx_callback);

  uip_ip6addr(&dest_addr, 0xfe80, 0, 0, 0, 0xf6ce, 0x365a, 0xb2be, 0x0274);

  // Initialize msg here after PROCESS_BEGIN
  msg.seq_num = 0;
  memset(msg.text, 0, BUFSIZE);  // Clear the buffer first
  strcpy(msg.text, "Mocan Ischia");

  SENSORS_ACTIVATE(temperature_sensor);

  while(1) {

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_timer));

    int raw_value = temperature_sensor.value(0);
    msg.temperature = raw_value;

    // Send the entire struct for simplicity
    LOG_INFO("Sending message seq_num: %d, temperature: %d, message: '%s'\n", msg.seq_num, msg.temperature, msg.text);
    LOG_INFO("Sending to ");
    LOG_INFO_6ADDR(&dest_addr);
    LOG_INFO_("\n");

    simple_udp_sendto(&udp_conn, &msg, sizeof(struct message), &dest_addr);

    msg.seq_num++;

    etimer_reset(&timer_timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
