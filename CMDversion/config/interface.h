/**
 * 通用的消息模板接口
 */


#ifndef INTERFACE_H
#define INTERFACE_H

typedef struct IMessageReceiver {
    void (*on_message)(void *self, const char *msg);
} IMessageReceiver;

#endif