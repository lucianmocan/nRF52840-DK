#include "contiki.h"
#include "net/routing/routing.h"
#include <stdlib.h>

/*---------------------------------------------------------------------------*/
PROCESS(rpl_root_process, "RPL DoDAG Root Process");
AUTOSTART_PROCESSES(&rpl_root_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(rpl_root_process, ev, data) 
{
    PROCESS_BEGIN();
   
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
