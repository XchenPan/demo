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