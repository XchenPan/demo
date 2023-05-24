#include "pcbqueue.h"
#include "thread.h"

void readyQueue(ngx_queue_t *ready_queue) {
    int round = 5;
    pcb_ready_queue_insert(ready_queue, "001", round, 56);
    pcb_ready_queue_insert(ready_queue, "002", round, 88);
    pcb_ready_queue_insert(ready_queue, "003", round, 32);
    pcb_ready_queue_insert(ready_queue, "004", round, 65);
    pcb_ready_queue_insert(ready_queue, "005", round, 42);
    // pcb_ready_queue_insert(ready_queue, "006", round, 23);
    // pcb_ready_queue_insert(ready_queue, "007", round, 20);
    // pcb_ready_queue_insert(ready_queue, "008", round, 62);
    // pcb_ready_queue_insert(ready_queue, "009", round, 45);
    // pcb_ready_queue_insert(ready_queue, "010", round, 18);
}
void runningQueue(ngx_queue_t *ready_queue, ngx_queue_t *running_queue, ngx_queue_t *finished_queue) {
    pcb_t *pcb = pcb_queue_pop_by_max_prio(ready_queue);

    pcb_queue_push(running_queue, pcb, p_running);
    pcb_t *node;
    node = pcb_queue_pop_only(running_queue);
    node->needtime = node->needtime - node->round;
    node->prio = 100 - node->needtime;
    pcb_queue_push(running_queue, node, p_running);
    pcb_t *pcb_pop = pcb_queue_pop_only(running_queue);

    pcb_queue_push_readyORfinished(ready_queue, finished_queue, pcb_pop);
}
void finishedQueue(ngx_queue_t *finished_queue, pcb_t *pcb) {
    pcb_queue_push(finished_queue, pcb, p_finished);
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

    for (int i = 0; i < 60; i++)
        runningQueue(&ready_queue, &running_queue, &finished_queue);
    printf("--------ready--------\n");
    get_all_pcb(&ready_queue);
    printf("--------finished--------\n");
    get_all_pcb(&finished_queue);
    return 0;
}