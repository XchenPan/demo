#ifndef __PCB__H__
#define __PCB__H__

#include "queue.h"

enum { p_ready = 0, p_running, p_finished, };       //进程的状态值

typedef struct pcb_s pcb_t;
struct pcb_s
{
    char *name;         //进程标识符
    int prio;           //进程优先数
    int round;          //进程时间片轮转时间片
    int cputime;        //进程占用CPU时间
    int needtime;       //进程到完成还需要的时间
    int count;          //计数器
    int state;          //进程的状态
    ngx_queue_t list;
};


#endif  /*__PCB__H__*/