#include "queue.h"
#include <stdlib.h>

queue_t* queue_init(void)
{
	queue_t* queue = malloc(sizeof(queue_t));
	*queue = (queue_t){ .list = list_init(), .size = 0 };
	return queue;
}

void queue_release(queue_t* queue)
{
	list_release(queue->list);
	free(queue);
}


void* queue_get(const queue_t* queue, const size_t idx) { return list_get(queue->list, idx); }
void queue_set(const queue_t* queue, void* value, const size_t idx) { list_set(queue->list, value, idx); }

list_node_t* queue_find(const queue_t* queue, void* target) { return list_find(queue->list, target); }
list_node_t* queue_rfind(const queue_t* queue, void* target) { return list_rfind(queue->list, target); }

void queue_push(queue_t* queue, void* value)
{
	list_push_back(queue->list, value);
	queue->size++;
}

void* queue_pop(queue_t* queue)
{
	queue->size--;
	return list_pop(queue->list, 0);
}

void queue_clear(queue_t* queue) { list_clear(queue->list); queue->size = 0; }
bool queue_is_empty(const queue_t* queue) { return queue->size == 0; }
