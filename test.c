#include "pcbqueue.h"
#include "thread.h"

static int round = 5;

void readyQueue_start(ngx_queue_t *ready_queue) {
    pcb_ready_queue_insert(ready_queue, "001", round, 56);
    pcb_ready_queue_insert(ready_queue, "002", round, 88);
    pcb_ready_queue_insert(ready_queue, "003", round, 32);
    pcb_ready_queue_insert(ready_queue, "004", round, 65);
}

void readyQueue_running_1(ngx_queue_t *ready_queue) {
    pcb_ready_queue_insert(ready_queue, "005", round, 32);
}

void readyQueue_running_2(ngx_queue_t *ready_queue) {
    pcb_ready_queue_insert(ready_queue, "006", round, 76);
    pcb_ready_queue_insert(ready_queue, "007", round, 43);
}

void readyQueue_running_3(ngx_queue_t *ready_queue) {
    pcb_ready_queue_insert(ready_queue, "008", round, 22);
    pcb_ready_queue_insert(ready_queue, "009", round, 45);
    pcb_ready_queue_insert(ready_queue, "010", round, 87);
}

void T1(ngx_queue_t *ready_queue, ngx_queue_t *running_queue, ngx_queue_t *finished_queue) {
    if (ngx_queue_empty(ready_queue))
        return;
    
    pcb_t *pcb = pcb_queue_pop_by_max_prio(ready_queue);

    pcb_queue_push(running_queue, pcb, p_running);
    get_all_pcb(running_queue);
    pcb_t *node;
    node = pcb_queue_pop_only(running_queue);
    if (node->needtime <= node->round) {
        node->needtime = 0;
        sleep(node->needtime * 0.01);
        pcb_queue_push_readyORfinished(ready_queue, finished_queue, node);
        return;
    }
    node->needtime = node->needtime - node->round;
    node->prio = 100 - node->needtime;
    sleep(0.2);
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
    
    readyQueue_start(&ready_queue);

    printf("--------ready--------\n");
    get_all_pcb(&ready_queue);
    printf("--------running--------\n");
    for (int i = 0; i < 1000; i++) {
        T1(&ready_queue, &running_queue, &finished_queue);
        if (i == 8) readyQueue_running_1(&ready_queue);
        if (i == 15) readyQueue_running_2(&ready_queue);
        if (i == 25) readyQueue_running_3(&ready_queue);

    }
    printf("--------finished--------\n");
    get_all_pcb(&finished_queue);
    return 0;
}