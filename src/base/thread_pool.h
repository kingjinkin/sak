/*
 * thread_pool.h
 *
 *  Created on: 2013-2-23
 *      Author: joey
 */

#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Defines the return values of all interface
 */
enum tagTHREAD_ERR_E
{
    THREAD_ERR_NONE = 0,
    THREAD_ERR_UNKONW = -1,
    THREAD_ERR_PARAM = -2,
    THREAD_ERR_MEM = -3,
};

/**
 * Defines the running status of current task
 *
 */
enum tagTHREAD_STATUS_E
{
    THREAD_STATUS_READY = 0,    /*added in the pool*/
    THREAD_STATUS_RUNNING,      /*already running in the pool*/
    THREAD_STATUS_STOP,         /*has finished*/
    THREAD_STATUS_CANCELED,     /*The thread canceled by owner*/
};

enum tagTHREAD_CONTROL_E
{
    THREAD_CONTROL_SETSIZE = 0, /*arg type is int*/
    THREAD_CONTROL_GETSIZE ,    /*arg type is int**/

};

typedef enum tagTHREAD_ERR_E THREAD_ERR_E;
typedef enum tagTHREAD_STATUS_E THREAD_STATUS_E;
typedef enum tagTHREAD_CONTROL_E THREAD_CONTROL_E;

/**
 * Note: for quick exiting, if proc in THREAD_STATUS_CANCELED state,
 *      it should exit itself.
 */

typedef struct tagTHREAD_POOL_S THREAD_POOL_S;
typedef struct tagTHREAD_ENTRY_S THREAD_ENTRY_S;
typedef void (*THREAD_CALLBACK)(THREAD_ENTRY_S* task);
/**
 * thread object
 */
struct tagTHREAD_ENTRY_S
{
    int (*proc)(void* priv);        /*thread entry point*/
    int (*cancel)(void * priv);     /*cancel proc internal*/
    void*               priv;       /*private parameter for proc*/
    int                 status;     /*value defined in THREAD_STATUS_E*/
    int                 result;     /*The return result of proc*/
    char*               name;       /*Name of proc*/
} ;

struct tagTHREAD_POOL_S
{
    int     (*control)(THREAD_POOL_S* self, int cmd, void* arg);
    int     (*addTask)(THREAD_POOL_S* self, THREAD_ENTRY_S* entry);
    int     (*delTask)(THREAD_POOL_S* self, THREAD_ENTRY_S* entry);
    int     (*realise)(THREAD_POOL_S* self, int num, THREAD_CALLBACK callback);
    void    (*destroy)(THREAD_POOL_S* self);
} ;

static int THREAD_POOL_Control(THREAD_POOL_S* self, int cmd, void* arg)
{
    if (self) {
        return self->control(self, cmd, arg);
    }
    return THREAD_ERR_PARAM;
}

static int THREAD_POOL_Realise(THREAD_POOL_S* self, int num, THREAD_CALLBACK callback)
{
    if (self) {
        return self->realise(self, num, callback);
    }
    return THREAD_ERR_PARAM;
}

static int THREAD_POOL_AddTask(THREAD_POOL_S* self, THREAD_ENTRY_S* entry)
{
    if (self && entry) {
        return self->addTask(self, entry);
    }
    return THREAD_ERR_PARAM;
}

/**
 * delete the task have added to the pool.
 * if the task is running, cancel from thread
 * if the task is waiting, just remove from queue
 */
static int THREAD_POOL_DelTask(THREAD_POOL_S* self, THREAD_ENTRY_S* entry)
{
    if (self && entry) {
        return self->delTask(self, entry);
    }
    return THREAD_ERR_PARAM;
}

static void THREAD_POOL_Destroy(THREAD_POOL_S* self)
{
    if (self) {
        return self->destroy(self);
    }
}

THREAD_POOL_S* THREAD_POOL_Create();

#ifdef __cplusplus
}
#endif
#endif /* THREAD_POOL_H_ */
