#include "pcbqueue.h"
#include "thread.h"

static int round = 5;
static int sign = 1;

void readyQueue(ngx_queue_t *ready_queue) {
    pcb_ready_queue_insert(ready_queue, "001", round, 56);
    pcb_ready_queue_insert(ready_queue, "002", round, 88);
    pcb_ready_queue_insert(ready_queue, "003", round, 32);
    pcb_ready_queue_insert(ready_queue, "004", round, 65);
    pcb_ready_queue_insert(ready_queue, "005", round, 92);
    pcb_ready_queue_insert(ready_queue, "006", round, 76);
    pcb_ready_queue_insert(ready_queue, "007", round, 43);
    pcb_ready_queue_insert(ready_queue, "008", round, 12);
    pcb_ready_queue_insert(ready_queue, "009", round, 45);
    pcb_ready_queue_insert(ready_queue, "010", round, 37);
    pcb_ready_queue_insert(ready_queue, "011", round, 45);
    pcb_ready_queue_insert(ready_queue, "012", round, 32);
    pcb_ready_queue_insert(ready_queue, "013", round, 76);
    pcb_ready_queue_insert(ready_queue, "014", round, 23);
    pcb_ready_queue_insert(ready_queue, "015", round, 54);
}

void T1(ngx_queue_t *ready_queue, ngx_queue_t *running_queue, ngx_queue_t *finished_queue) {
    if (ngx_queue_empty(ready_queue)) {
        sign = 0;
        return;
    }
    pcb_t *pcb = pcb_queue_pop_by_max_prio(ready_queue);

    pcb_queue_push(running_queue, pcb, p_running);
    get_all_pcb(running_queue);
    pcb_t *node;
    node = pcb_queue_pop_only(running_queue);
    if (node->needtime <= node->round) {
        node->needtime = 0;
        pcb_queue_push_readyORfinished(ready_queue, finished_queue, node);
        return;
    }
    node->needtime = node->needtime - node->round;
    node->prio = node->prio - round;
    pcb_queue_prio_up(ready_queue);
    pcb_queue_push_readyORfinished(ready_queue, finished_queue, node);
}

int main(int argc, char const *argv[])
{
    ngx_queue_t ready_queue;
    ngx_queue_t running_queue;
    ngx_queue_t finished_queue;

    ngx_queue_init(&ready_queue);
    ngx_queue_init(&running_queue);
    ngx_queue_init(&finished_queue);
    
    readyQueue(&ready_queue);

    printf("--------ready--------\n");
    get_all_pcb(&ready_queue);
    printf("--------running--------\n");
    while (sign) 
        T1(&ready_queue, &running_queue, &finished_queue);
    printf("--------finished--------\n");
    get_all_pcb(&finished_queue);
    return 0;
}