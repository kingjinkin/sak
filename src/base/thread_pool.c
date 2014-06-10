/*
 * thread_pool.c
 *
 *  Created on: 2013-2-23
 *      Author: joey
 */

#include <pthread.h>
#include "common.h"
#include "list.h"
#include "thread_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _tagTHREAD_POOL_S
{
    THREAD_POOL_S       super;
    /*private*/
    LIST_S              wait_queue;
    LIST_S              work_queue;
    LIST_S              task_queue;
    pthread_mutex_t     lock;
    pthread_cond_t      cond;
    THREAD_CALLBACK     task_finish_notify;
    int                 exit;
    int                 decrease_size;
} _THREAD_POOL_S;

typedef struct _tagTHREAD_QUEUE_S
{
    LIST_S              list;
    pthread_t           thread;
    _THREAD_POOL_S*     pool;
    THREAD_ENTRY_S*     entry;
} THREAD_QUEUE_S;

static inline void THREAD_QUEUE_Init(THREAD_QUEUE_S* self, _THREAD_POOL_S* pool)
{
    LIST_Init(&self->list);
    self->pool = pool;
}

typedef struct _tagTASK_QUEUE_S
{
    LIST_S              list;
    THREAD_ENTRY_S*     entry;
    THREAD_QUEUE_S*     thread_queue; /*the thread which execute this task*/
} TASK_QUEUE_S;

static inline void TASK_QUEUE_Init(TASK_QUEUE_S* self, THREAD_ENTRY_S* entry)
{
    LIST_Init(&self->list);
    self->entry = entry;
}



//--------------------------------------------------------------------
#define DECLARE_POOL(name, from) _THREAD_POOL_S* name = (_THREAD_POOL_S*)(from)
//--------------------------------------------------------------------
/**
 * all pthread using this proc
 */
static void* thread_proc(void* data)
{
    THREAD_QUEUE_S* thread_queue = (THREAD_QUEUE_S*)data;
    _THREAD_POOL_S* pool = thread_queue->pool;
    int ret;
    while (!pool->exit) {
        ret = pthread_mutex_lock(&pool->lock);
        CHECK(ret, return NULL, "pthread_mutex_lock failed");

        while (LIST_IsEmpty(&pool->task_queue)) {
            ret = pthread_cond_wait(&pool->cond, &pool->lock);
            CHECK(ret, return NULL, "pthread_cond_wait failed");
            if(pool->exit) {
                ret = pthread_mutex_unlock(&pool->lock);
                CHECK(ret, return NULL, "pthread_mutex_unlock failed");
                LOGD("thread canceled");
                return NULL;
            }
        }

        /*Remove from idle queue, then add to work queue*/
        LIST_S *list = &thread_queue->list;
        LIST_Del(list);
        LIST_AddTail(&pool->work_queue, list);

        /*Remove task from task queue*/
        list = pool->task_queue.next;
        LIST_Del(list);
        ret = pthread_mutex_unlock(&pool->lock);
        CHECK(ret, return NULL, "pthread_mutex_unlock failed");

        /*Going to work*/
        TASK_QUEUE_S *task = CONTAINER_OF(list,  list, TASK_QUEUE_S);
        if (task->entry) {
            THREAD_ENTRY_S* entry = task->entry;
            task->thread_queue = thread_queue;
            thread_queue->entry = entry;
            entry->status  = THREAD_STATUS_RUNNING;
            LOGD("thread going to executing task:%s", entry->name);
            entry->result = entry->proc(entry->priv);
            entry->status  = THREAD_STATUS_STOP;
            pool->task_finish_notify(entry);
            thread_queue->entry = NULL;
            FREE(task);
        }
        else {
            LOGE("Empty thread entry, this case should not happen!");
        }

        if (pool->exit) {
            LOGD("thread canceled, do not need to add to idle queue");
            return NULL;
        }
        /*Finishing work, add current thread to idle queue*/
        ret = pthread_mutex_lock(&pool->lock);
        CHECK(ret, return NULL, "pthread_mutex_lock failed");
        list = &thread_queue->list;
        LIST_Del(list);
        LIST_AddTail(&pool->wait_queue, list);
        ret = pthread_mutex_unlock(&pool->lock);
        CHECK(ret, return NULL, "pthread mutex unlock failed");

        LOGD("thread going to wait a new task");
    }
    return NULL;
}

static int _THREAD_POOL_CreateThread(_THREAD_POOL_S* pool, int num)
{
    int i, ret;
    THREAD_QUEUE_S* qe;
    for (i = 0; i < num; i++) {
        qe = NEW(THREAD_QUEUE_S);
        CHECK(!qe, return THREAD_ERR_MEM, "Allocate thread queue failed");
        THREAD_QUEUE_Init(qe, pool);
        ret = pthread_create(&qe->thread, NULL, thread_proc, qe);
        CHECK (ret, {
            FREE(qe);
            return THREAD_ERR_UNKONW;
        }, "pthread create failed");
        /*add thread to idle queue*/
        LOGD("The %dth thread created successfully", i);
        LIST_AddTail(&pool->wait_queue, &qe->list);
    }
    return THREAD_ERR_NONE;
}

static inline void _THREAD_POOL_DestroyIdleThread(_THREAD_POOL_S* pool)
{
    int ret;
    LIST_S *pos;
    LIST_FOREACH(pos, &pool->wait_queue)
    {
        LOGD("Trying to cancel idle thread");
        THREAD_QUEUE_S *thread_queue = CONTAINER_OF(pos, list, THREAD_QUEUE_S);
        ret = pthread_join(thread_queue->thread, NULL );
        CHECK(ret, , "pthread_join failed");
        FREE(thread_queue);
    }
}

static inline int _THREAD_POOL_GetSize(_THREAD_POOL_S* pool)
{
    LIST_S* pos;
    int size = 0;
    LIST_FOREACH(pos, &pool->work_queue)
    {
        size++;
    }
    LIST_FOREACH(pos, &pool->wait_queue)
    {
        size++;
    }
    return size;
}

static inline int _THREAD_POOL_SetSize(_THREAD_POOL_S* pool, int size)
{
    int diff = size - _THREAD_POOL_GetSize(pool);
    int ret;
    if (diff > 0)
    {
        LOGD("Going to increase %d threads", diff);
        return _THREAD_POOL_CreateThread(pool, diff);
    } else if (diff < 0) {
        LOGD("Going to decrease %d threads", diff);
        ret = pthread_mutex_lock(&pool->lock);
        CHECK(ret, return THREAD_ERR_UNKONW, "pthread_mutex_lock failed");

        _THREAD_POOL_DestroyIdleThread(pool);

        ret = pthread_mutex_unlock(&pool->lock);
        CHECK(ret, return THREAD_ERR_UNKONW, "pthread_mutex_unlock failed");
        return _THREAD_POOL_GetSize(pool);
    } else {
        LOGD("Threads number no changed");
    }
    return THREAD_ERR_NONE;
}

//--------------------------------------------------------------------
static int _THREAD_POOL_Control(THREAD_POOL_S* self, int cmd, void* arg)
{
    DECLARE_POOL(pool, self);

    switch (cmd) {
    case THREAD_CONTROL_SETSIZE: {
        CHECK(!arg, return THREAD_ERR_PARAM, "Invalid size");
        return _THREAD_POOL_SetSize(pool, *(int*)arg);
        break;
    }
    case THREAD_CONTROL_GETSIZE: {
        *(int*)arg = _THREAD_POOL_GetSize(pool);
        break;
    }
    default: {
        LOGE("unsupported control cmd:%d", cmd);
        return THREAD_ERR_UNKONW;
        break;
    }
    }

    return THREAD_ERR_NONE;
}
static int _THREAD_POOL_DelTask(THREAD_POOL_S* self, THREAD_ENTRY_S* entry)
{
    DECLARE_POOL(pool, self);
    int ret;
    switch (entry->status) {
    case THREAD_STATUS_RUNNING: {
        ret = entry->cancel(entry->priv);
        CHECK(ret, return THREAD_ERR_UNKONW, "cancel proc failed");
        break;
    }
    case THREAD_STATUS_CANCELED: {
        LOGD("the task is already canceled");
        break; //fall through
    }
    case THREAD_STATUS_STOP: {
        LOGD("the task is stopped");
       break; //fall through
    }
    case THREAD_STATUS_READY: {
        ret = pthread_mutex_lock(&pool->lock);
        CHECK(ret, return THREAD_ERR_UNKONW, "pthread_mutex_lock failed");
        LIST_S* pos;
        LIST_FOREACH(pos, &pool->task_queue) {
            TASK_QUEUE_S *task = CONTAINER_OF(pos,  list, TASK_QUEUE_S);
            if (task->entry == entry) {
                LIST_Del(pos);
                FREE(task);
                ret = pthread_mutex_unlock(&pool->lock);
                CHECK(ret, return THREAD_ERR_UNKONW, "pthread_mutex_unlock failed");

                pool->task_finish_notify(entry);
                entry->status = THREAD_STATUS_CANCELED;
            }
        }
        ret = pthread_mutex_unlock(&pool->lock);
        CHECK(ret, return THREAD_ERR_UNKONW, "pthread_mutex_unlock failed");
        break;
    }
    default: {
        LOGE("Can not delete task %s from pool", entry->name);
        return THREAD_ERR_UNKONW;
        break;
    }
    }
    return THREAD_ERR_NONE;
}

/**
 * sequence:
 * create_internal_task->add_task->notify_wait_thread
 */
static int _THREAD_POOL_AddTask(THREAD_POOL_S* self, THREAD_ENTRY_S* entry)
{
    DECLARE_POOL(pool, self);
    int ret;
    TASK_QUEUE_S* task_queue = NEW(TASK_QUEUE_S);
    CHECK(!task_queue, return THREAD_ERR_MEM, "Allocate thread queue failed");
    TASK_QUEUE_Init(task_queue, entry);

    ret = pthread_mutex_lock(&pool->lock);
    CHECK(ret, return THREAD_ERR_UNKONW, "pthread_mutex_lock failed");
    LIST_AddTail(&pool->task_queue, &task_queue->list);
    entry->status = THREAD_STATUS_READY;
    ret = pthread_mutex_unlock(&pool->lock);
    CHECK(ret, return THREAD_ERR_UNKONW, "pthread_mutex_lock failed");

    ret = pthread_cond_signal(&pool->cond);
    CHECK(ret, return THREAD_ERR_UNKONW, "pthread_cond_signal failed");

    return THREAD_ERR_NONE;
}

static int _THREAD_POOL_Realise(THREAD_POOL_S* self, int num, THREAD_CALLBACK callback)
{
    DECLARE_POOL(pool, self);
    int ret;

    ret = _THREAD_POOL_CreateThread(pool, num);
    CHECK(ret != THREAD_ERR_NONE, return THREAD_ERR_UNKONW, "_THREAD_POOL_CreateThread failed");

    pool->task_finish_notify = callback;
    return THREAD_ERR_NONE;
}

static void _THREAD_POOL_Destroy(THREAD_POOL_S* self)
{
    DECLARE_POOL(pool, self);
    int ret;
    /*Destroy all threads*/
    pool->exit = 1;
    LIST_S *pos;
    ret = pthread_cond_broadcast(&pool->cond);
    CHECK(ret, , "pthread_cond_broadcast failed");

    LIST_FOREACH(pos, &pool->work_queue) {
        LOGD("Trying to cancel work thread");
        THREAD_QUEUE_S *thread_queue = CONTAINER_OF(pos, list, THREAD_QUEUE_S);
        THREAD_ENTRY_S* entry = thread_queue->entry;
        if (entry) {
            _THREAD_POOL_DelTask(self, entry);
        }
        ret = pthread_join(thread_queue->thread, NULL);
        CHECK(ret, , "pthread_join failed");
        FREE(thread_queue);
    }

    _THREAD_POOL_DestroyIdleThread(pool);

    LIST_FOREACH(pos, &pool->task_queue) {
        TASK_QUEUE_S* task = CONTAINER_OF(pos, list, TASK_QUEUE_S);
        THREAD_ENTRY_S* entry = task->entry;
        if (entry && task->thread_queue) {
            LOGD("Trying to cancel working thread, task:%s", entry->name);
            entry->cancel(entry->priv); /*cancel work proc*/
            LOGD("here");
            entry->status = THREAD_STATUS_CANCELED;
            ret = pthread_join(task->thread_queue->thread, NULL);
            CHECK(ret, NO_OP, "pthread_join failed");
            LIST_Del(&task->thread_queue->list);
        }
        pool->task_finish_notify(entry);
        FREE(task);
    }

    ret = pthread_mutex_destroy(&pool->lock);
    CHECK(ret, NO_OP, "pthread_mutex_destroy failed");
    ret = pthread_cond_destroy(&pool->cond);
    CHECK(ret, NO_OP, "pthread_cond_destroy failed");
    FREE(pool);
}

/**
 * sequence:
 * newobj->init_interface->init_lock_cond
 */
THREAD_POOL_S* THREAD_POOL_Create()
{
    int ret;
    _THREAD_POOL_S* pool = NEW(_THREAD_POOL_S);
    CHECK(!pool, return NULL, "Allocate thread pool failed");

    pool->super.addTask = _THREAD_POOL_AddTask;
    pool->super.delTask = _THREAD_POOL_DelTask;
    pool->super.destroy = _THREAD_POOL_Destroy;
    pool->super.realise = _THREAD_POOL_Realise;
    pool->super.control = _THREAD_POOL_Control;

    LIST_Init(&pool->task_queue);
    LIST_Init(&pool->wait_queue);
    LIST_Init(&pool->work_queue);

    ret = pthread_mutex_init(&pool->lock, NULL);
    CHECK(ret, {
            FREE(pool);
            return NULL;
    }, "pthread_mutex_init failed");

    ret = pthread_cond_init(&pool->cond, NULL);
    CHECK(ret, {
            pthread_mutex_destroy(&pool->lock);
            FREE(pool);
            return NULL;
    }, "pthread_cond_init failed");

    return &pool->super;
}

#ifdef cplusplus
}
#endif
