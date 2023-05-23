#include "pcbqueue.h"

int main(int argc, char const *argv[])
{
    ngx_queue_t ready_queue;
    ngx_queue_t running_queue;
    ngx_queue_t finished_queue;

    ngx_queue_init(&ready_queue);
    ngx_queue_init(&running_queue);
    ngx_queue_init(&finished_queue);

    pcb_ready_queue_insert(&ready_queue, "001", 5, 56);
    pcb_ready_queue_insert(&ready_queue, "002", 5, 98);
    pcb_ready_queue_insert(&ready_queue, "003", 5, 32);
    pcb_ready_queue_insert(&ready_queue, "004", 5, 65);
    pcb_ready_queue_insert(&ready_queue, "005", 5, 42);
    pcb_ready_queue_insert(&ready_queue, "006", 5, 23);
    pcb_ready_queue_insert(&ready_queue, "007", 5, 20);
    pcb_ready_queue_insert(&ready_queue, "008", 5, 62);
    pcb_ready_queue_insert(&ready_queue, "009", 5, 45);
    pcb_ready_queue_insert(&ready_queue, "010", 5, 18);

    
    
    get_all_pcb(&ready_queue);
    return 0;
}