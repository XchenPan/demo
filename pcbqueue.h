#ifndef __PCBQUEUE_H_
#define __PCBQUEUE_H_

#include "pcb.h"
#include <stdlib.h>
#include <memory.h>
#include <stddef.h>
#include <stdio.h>

void get_all_pcb(ngx_queue_t *list) {//打印队列
    ngx_queue_t *q;
    pcb_t *node;

    if (ngx_queue_empty(list)) {
        printf("该队列为空!\n");
        return;
    }
    for (q = ngx_queue_head(list); q != ngx_queue_sentinel(list); q = ngx_queue_next(q)) {
        node = ngx_queue_data(q, pcb_t, list);
        printf("name: %s     prio: %d    needtime: %d    state: %d\n", node->name, node->prio, node->needtime, node->state);
    }
}

ngx_queue_t* get_queue_max_prio_node(ngx_queue_t *list) {
    int max_prio = -1;
    ngx_queue_t *q;
    ngx_queue_t *target_node;
    pcb_t *node;
    for (q = ngx_queue_head(list); q != ngx_queue_sentinel(list); q = ngx_queue_next(q)) {
        node = ngx_queue_data(q, pcb_t, list);
        if (node->prio >= max_prio) {
            max_prio = node->prio;
            target_node = q;
        }
    }
    return target_node;
}

int pcb_ready_queue_insert(ngx_queue_t *list, char *name, int round, int cputime) {
    pcb_t *pcb = (pcb_t*)malloc(sizeof(pcb_t));
    if (pcb == NULL) {
        return -1;
    }
    memset(pcb, 0, sizeof(pcb_t));
    pcb->name = name;
    pcb->round = round;
    pcb->cputime = cputime;
    pcb->needtime = cputime;
    pcb->prio = 100 - pcb->needtime;
    pcb->state = p_ready;
    ngx_queue_insert_tail(list, &pcb->list);
    return 0;
}

int pcb_queue_push(ngx_queue_t *list, pcb_t *pcb, int state) {
    if (pcb == NULL) {
        return -1;
    }
    pcb->state = state;
    ngx_queue_insert_head(list, &pcb->list);
    return 0;
}

void pcb_queue_push_readyORfinished(ngx_queue_t *ready_queue, ngx_queue_t *finished_queue, pcb_t *pcb) {
    if (pcb->needtime == 0) {
        pcb->prio = -1;
        pcb_queue_push(finished_queue, pcb, p_finished);
    }
    else {
        pcb_queue_push(ready_queue, pcb, p_ready);
    }
}

pcb_t *pcb_queue_pop_by_max_prio(ngx_queue_t *list) {
    ngx_queue_t *q;
    ngx_queue_t *target_node;
    target_node = get_queue_max_prio_node(list);

    for (q = ngx_queue_head(list); q != ngx_queue_sentinel(list); q = ngx_queue_next(q)) {
        if (q == target_node) {
            ngx_queue_remove(q);
            break;
        }
    }
    return ngx_queue_data(q, pcb_t, list);
}

pcb_t *pcb_queue_pop_only(ngx_queue_t *list) {
    ngx_queue_t *q;
    if (ngx_queue_empty(list)) {
        return NULL;
    }
    q = ngx_queue_head(list);
    ngx_queue_remove(q);
    return ngx_queue_data(q, pcb_t, list);
}
#endif  /*__PCBQUEUE_H_*/