#pragma once
#include <stddef.h>
#include "../bool.h"

typedef struct list_node_t
{
	void* value;
	struct list_node_t* prev;
	struct list_node_t* next;
} list_node_t;

typedef struct
{
	list_node_t* head;
	list_node_t* tail;
	size_t size;
} list_t;


list_t* list_init(void);
void list_release(list_t*);

void* list_get(const list_t* list, size_t idx);
void list_set(const list_t* list, void* value, size_t idx);

list_node_t* list_find(const list_t* list, void* target);
list_node_t* list_rfind(const list_t* list, void* target);

void list_push(list_t* list, void* value, size_t idx);
void list_push_front(list_t* list, void* value);
void list_push_back(list_t* list, void* value);

void* list_pop(list_t* list, size_t idx);
void* list_pop_front(list_t* list);
void* list_pop_back(list_t* list);

void list_clear(list_t* list);
bool list_is_empty(const list_t* list);