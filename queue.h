#include <stdio.h>
#include <stdlib.h>
#include "PCB.h"

typedef struct PCB
{
    char *name;         //进程标识符
    int prio;           //进程优先数
    int round;          //进程时间片轮转时间片
    int cputime;        //进程占用CPU时间
    int needtime;       //进程到完成还需要的时间
    int count;          //计数器
    char *state;        //进程的状态
    struct node* next;  //链指针
}PCB;

typedef struct PCBQueue {
	PCB* first;
	PCB* Last;
	int length;
}PCBQueue;

void Queueinit(PCBQueue* queue) {
	if (queue == NULL) {
		return;
	}
	queue->length = 0;
	queue->Last = (PCB*)malloc(sizeof(PCB));
	queue->Last->next = NULL;
	queue->first = queue->Last;
}
 
PCBQueue* QueueCreate() {
	PCBQueue* pcbqueue = (PCBQueue*)malloc(sizeof(PCBQueue));
    Queueinit(pcbqueue);
    return pcbqueue;
}

PCBQueue* QueueInsert(PCBQueue* pcbqueue, char *name, int prio, int round, int cputime, int needtime, int count, char *state) {
	PCB* newPcb = (PCB*)malloc(sizeof(PCB));
    newPcb->name = name;
    newPcb->prio = prio;
    newPcb->round = round;
    newPcb->cputime = cputime;
    newPcb->needtime = needtime;
    newPcb->count = count;
    newPcb->state = state;
    newPcb->next =NULL;
    if (pcbqueue->Last == NULL) {
        pcbqueue->Last = pcbqueue->first = newPcb;
        pcbqueue->length++;
    }
    else {
        while (pcbqueue->Last->next != NULL)
            pcbqueue->Last = pcbqueue->Last->next;
        pcbqueue->Last->next = newPcb;
        pcbqueue->length++;
    }
}

void EnterQueue(PCBQueue* pcbqueue, PCB* pcb) {
    PCBQueue* p = NULL;
    p->Last = pcbqueue->first;
}