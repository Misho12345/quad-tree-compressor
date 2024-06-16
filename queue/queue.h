#pragma once
#include "../list/list.h"

typedef struct 
{
	list_t* list;
	size_t size;
} queue_t;

queue_t* queue_init(void);
void queue_release(queue_t* queue);

void* queue_get(const queue_t* queue, size_t idx);
void queue_set(const queue_t* queue, void* value, size_t idx);

list_node_t* queue_find(const queue_t* queue, void* target);
list_node_t* queue_rfind(const queue_t* queue, void* target);

void queue_push(queue_t* queue, void* value);
void* queue_pop(queue_t* queue);

void queue_clear(queue_t* queue);
bool queue_is_empty(const queue_t* queue);
