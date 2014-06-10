/*
 * list.h
 *
 *  Created on: 2013-2-23
 *      Author: joey
 */

#ifndef LIST_H_
#define LIST_H_

typedef struct LIST_S
{
	struct LIST_S 		*prev;
	struct LIST_S		*next;
} LIST_S;


//--------------------------------------------------------------------
#define LIST_FOREACH(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)
//--------------------------------------------------------------------
static inline void _LIST_Del(LIST_S *prev, LIST_S *next)
{
	prev->next = next;
	next->prev = prev;
}
static inline void _LIST_AddTail(LIST_S *prev, LIST_S *next, LIST_S *entry)
{
	prev->next = entry;
	entry->next = next;
	entry->prev = prev;
	next->prev = entry;
}
//--------------------------------------------------------------------
static inline void LIST_Init(LIST_S *list)
{
	list->next = list->prev = list;
}

static inline int LIST_IsEmpty(LIST_S *list)
{
	return list->next == list;
}

static inline void LIST_Del(LIST_S* list)
{
	_LIST_Del(list->prev, list->next);
}

static inline void LIST_AddTail(LIST_S* head, LIST_S* entry)
{
	_LIST_AddTail(head->prev, head, entry);
}
#endif /* LIST_H_ */
