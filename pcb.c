#include "pcb.h"
#include <stdlib.h>
#include <memory.h>
#include <stddef.h>
#include <stdio.h>

void get_all_pcb(ngx_queue_t *list) {//打印队列
    ngx_queue_t *q;
    pcb_t *node;

    for (q = ngx_queue_head(list);
        q != ngx_queue_sentinel(list);
        q = ngx_queue_next(q)) {
        
        node = ngx_queue_data(q, pcb_t, list);

        printf("%s %d\n", node->name, node->prio);
    }
}

int get_queue_count(ngx_queue_t *list) {
    int count = 0;
    ngx_queue_t *q;
    for (q = ngx_queue_head(list);
        q != ngx_queue_sentinel(list);
        q = ngx_queue_next(q))
        count++;
    return count;
}

ngx_queue_t* get_queue_max_prio_node(ngx_queue_t *list) {
    int max_prio = -1;
    ngx_queue_t *q;
    ngx_queue_t *target_node;
    pcb_t *node;
    for (q = ngx_queue_head(list);
        q != ngx_queue_sentinel(list);
        q = ngx_queue_next(q)) {
            node = ngx_queue_data(q, pcb_t, list);
            if (node->prio >= max_prio)
                target_node = q;
        }
    return target_node;
}

int pcb_ready_queue_insert(ngx_queue_t *list, char *name, int round, int cputime) {
    pcb_t *pcb = malloc(sizeof(pcb_t));
    if (pcb == NULL) {
        return -1;
    }
    memset(pcb, 0, sizeof(pcb_t));
    pcb->name = name;
    pcb->prio = 50 - pcb->needtime;
    pcb->round = round;
    pcb->cputime = cputime;
    pcb->needtime = cputime;
    pcb->state = p_ready;
    pcb->count = get_queue_count(list);
    ngx_queue_insert_tail(list, &pcb->list);
    return 0;
}

int pcb_queue_push(ngx_queue_t *list, pcb_t *pcb, int state) {
    if (pcb == NULL) {
        return -1;
    }
    pcb->state = state;
    pcb->count = get_queue_count(list);
    ngx_queue_insert_head(list, &pcb->list);
    return 0;
}

pcb_t *pcb_queue_pop(ngx_queue_t *list) {
    ngx_queue_t *q;
    ngx_queue_t *target_node;
    target_node = get_queue_max_prio_node(list);

    if (ngx_queue_empty(list)) {
        return NULL;
    }

    for (q = ngx_queue_head(list);
        q != ngx_queue_sentinel(list);
        q = ngx_queue_next(q)) {
        if (q == target_node) {
            ngx_queue_remove(q);
            break;
        }
    }
    return ngx_queue_data(q, pcb_t, list);
}

int main(int argc, char const *argv[])
{
    ngx_queue_t ready_queue;
    ngx_queue_t running_queue;
    ngx_queue_t finished_queue;

    ngx_queue_init(&ready_queue);
    ngx_queue_init(&running_queue);
    ngx_queue_init(&finished_queue);

    pcb_ready_queue_insert(&ready_queue, "panchen", 50, 50);
    pcb_ready_queue_insert(&ready_queue, "bapie", 70, 50);
    pcb_ready_queue_insert(&ready_queue, "panchen", 20, 50);
    pcb_ready_queue_insert(&ready_queue, "panchen", 43, 50);
    pcb_ready_queue_insert(&ready_queue, "bapie", 34, 50);
    pcb_ready_queue_insert(&ready_queue, "hello", 23, 50);
    pcb_queue_pop(&ready_queue);
    

    get_all_pcb(&ready_queue);
    return 0;
}