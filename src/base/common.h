/*
 * common.h
 *
 *  Created on: 2013-2-23
 *      Author: joey
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_OP

#define NEW_N(type, n) (typeof(type)*)(calloc((n), sizeof(type)))
#define NEW(type) NEW_N(type, 1)
#define FREE(ptr) if (ptr) free(ptr)
//--------------------------------------------------------------------

#define OFFSET_OF(member, type) (&((typeof(type)*)0)->member)
#define CONTAINER_OF(ptr, member, type) \
	(typeof(type)*)((char*)ptr - (char*)OFFSET_OF(member, type))

/**
 * Log utilizes
 */
#define LOGD(fmt, args...) \
	printf("DEBUG [%s,%d]"#fmt"\n", __FILE__, __LINE__, ##args)
#define LOGE(fmt, args...) \
    printf("ERROR [%s,%d]"#fmt"\n", __FILE__, __LINE__, ##args)
#define CHECK(cond, action, fmt, args...)\
        if (cond) {\
            printf("Check "#cond" failed, at [%s,%d], we will "#action" message:"#fmt"\n",\
                __FILE__, __LINE__, ##args);\
            action;\
        }

/**
 * critical define
 */
#define CRITICAL_STATUS_OK (0)
#define CRITICAL_STATUS_ERR (-1)
#define CRITICAL_ENTER() \
    int __critical_status = CRITICAL_STATUS_OK;\
    do {
#define CRITICAL_LEAVE() } while (0);
#define LEAVE_CRITICAL __critical_status = CRITICAL_STATUS_ERR;break
#define TEST_CRITICAL_STATUS() __critical_status == CRITICAL_STATUS_OK
#endif /* COMMON_H_ */
