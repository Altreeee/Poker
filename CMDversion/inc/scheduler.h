#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "config/interface.h"


/*
typedef struct{
    IMessageReceiver *send_target[10]; // 发送目标模块列表
    void (*send)(void *self, MSG msg); // 发送消息的方法
} Scheduler;

Scheduler new_scheduler(void);
*/

void wakeUpScheduler(void);


#endif // SCHEDULER_H