#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include <stdio.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
PROCESS(unicast_process, "Unicast on Timeout");
AUTOSTART_PROCESSES(&unicast_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_process, ev, data)
{
    PROCESS_BEGIN();

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
