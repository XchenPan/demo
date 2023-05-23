#include "pcb.h"
#include <stdlib.h>
#include <memory.h>
#include <stddef.h>
#include <stdio.h>

int get_all_pcb(ngx_queue_t *list) {
    ngx_queue_t *q;
    pcb_t *node;

    for (q = ngx_queue_head(list);
        q != ngx_queue_sentinel(list);
        q = ngx_queue_next(q)) {
        
        node = ngx_queue_data(q, pcb_t, list);

        printf("%d\n", node->cputime);
    }
}

int pcb_push(ngx_queue_t *list, int cpu_time) {
    pcb_t *pcb = malloc(sizeof(pcb_t));
    if (pcb == NULL) {
        return EXIT_FAILURE;
    }
    memset(pcb, 0, sizeof(pcb_t));

    pcb->cputime = cpu_time;

    ngx_queue_insert_tail(list, &pcb->list);
    return 0;
}

//
int main(int argc, char const *argv[])
{
    ngx_queue_t list;
    ngx_queue_init(&list);

    
    pcb_push(&list, 1);

    pcb_push(&list, 2);
   

    get_all_pcb(&list);
    return 0;
}
