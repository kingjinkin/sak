/*
 * ThreadPoolTest.cpp
 *
 *  Created on: 2013-3-2
 *      Author: joey
 */

#include <gtest.h>
#include <thread_pool.h>
#include <common.h>

struct task_ctx
{
    int exit;
    char name[64];
};

static void pool_callback(THREAD_ENTRY_S* task)
{
    LOGD("pool_callback:%s", task->name);
}

static int task_cancel(void* priv)
{
    struct task_ctx* ctx = (struct task_ctx*)priv;
    LOGD("task_cancel:%s", ctx->name);
    ctx->exit = 1;
    return 0;
}

static int task_proc(void* priv)
{
    struct task_ctx* ctx = (struct task_ctx*)priv;
    while (!ctx->exit)
    {
        LOGD("task :%s is running", ctx->name);
        sleep(1);
    }
    return 0;
}

TEST(ThreadPoolTest, testCreateDestroy) {
    THREAD_POOL_S* pool = THREAD_POOL_Create();
    EXPECT_TRUE(pool);

    int ret = THREAD_POOL_Realise(pool, 1024, pool_callback);
    EXPECT_EQ(ret, THREAD_ERR_NONE);

    THREAD_POOL_Destroy(pool);
}

TEST(ThreadPoolTest, testTask) {
    THREAD_POOL_S* pool = THREAD_POOL_Create();
    EXPECT_TRUE(pool);

    int ret = THREAD_POOL_Realise(pool, 10, pool_callback);
    EXPECT_EQ(ret, THREAD_ERR_NONE);

    const int task_num = 10;
    THREAD_ENTRY_S tasks[task_num] = {0};
    struct task_ctx ctx[task_num] = {0};

    for (int i = 0; i < task_num; i++)
    {
        sprintf(ctx[i].name, "name-%d", i);
        ctx[i].exit = 0;
        tasks[i].cancel = task_cancel;
        tasks[i].name = ctx[i].name;
        tasks[i].proc = task_proc;
        tasks[i].priv = ctx + i;
        ret = THREAD_POOL_AddTask(pool, tasks + i);
        EXPECT_EQ(ret, THREAD_ERR_NONE);
    }

    sleep(1);
    THREAD_POOL_Destroy(pool);
}
