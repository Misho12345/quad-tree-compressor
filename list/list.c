#include "list.h"
#include <stdlib.h>

static list_node_t* get_node(const list_t* list, size_t index)
{
	if (index > list->size)
		index = list->size;

	list_node_t* node;

	if (index < list->size / 2)
	{
		node = list->head;

		for (size_t i = 0; i < index; i++)
			node = node->next;
	}
	else
	{
		node = list->tail;
		const size_t l = list->size - index - 1;

		for (size_t i = 0; i < l; i++)
			node = node->prev;
	}

	return node;
}

static list_node_t* new_node(void* value)
{
	list_node_t* node = malloc(sizeof(list_node_t));
	*node = (list_node_t) { .value = value, .prev = NULL, .next = NULL };
	return node;
}


list_t* list_init(void)
{
	list_t* list = malloc(sizeof(list_t));
	*list = (list_t){ .head = NULL, .tail = NULL, .size = 0 };
	return list;
}

void list_release(list_t* list)
{
	list_node_t* node = list->head;
	while (node != NULL) 
	{
		list_node_t* prev = node;
		node = node->next;
		free(prev);
	}
	free(list);
}


void* list_get(const list_t* list, const size_t idx)
{
	return get_node(list, idx)->value;
}

void list_set(const list_t* list, void* value, const size_t idx)
{
	get_node(list, idx)->value = value;
}


list_node_t* list_find(const list_t* list, void* target)
{
	for (list_node_t* node = list->head; node; node = node->next)
		if (node->value == target) 
			return node;

	return NULL;
}

list_node_t* list_rfind(const list_t* list, void* target)
{
	for (list_node_t* node = list->tail; node; node = node->prev)
		if (node->value == target)
			return node;

	return NULL;
}


void list_push(list_t* list, void* value, const size_t idx)
{
	if (idx <= 0) list_push_front(list, value);
	else if (idx >= list->size) list_push_back(list, value);
	else
	{
		list_node_t* node = get_node(list, idx);

		list_node_t* prev = node->prev;
		node->prev = new_node(value);
		node->prev->next = node;
		node->prev->prev = prev;
		node->prev->prev->next = node->prev;

		list->size++;
	}
}

void list_push_front(list_t* list, void* value)
{
	list_node_t* head = list->head;

	list->head = new_node(value);
	list->head->next = head;

	if (list->size != 0) head->prev = list->head;
	else list->tail = list->head;

	list->size++;
}

void list_push_back(list_t* list, void* value)
{
	list_node_t* tail = list->tail;

	list->tail = new_node(value);
	list->tail->prev = tail;

	if (list->size != 0) tail->next = list->tail;
	else list->head = list->tail;

	list->size++;
}


void* list_pop(list_t* list, const size_t idx)
{
	if (idx <= 0) return list_pop_front(list);
	if (idx >= list->size - 1) return list_pop_back(list);

	if (list->size == 0) return NULL;

	list_node_t* node = get_node(list, idx);
	void* node_value = node->value;

	node->prev->next = node->next;
	node->next->prev = node->prev;

	free(node);
	list->size--;

	return node_value;
}

void* list_pop_front(list_t* list)
{
	if (list->size == 0) return NULL;

	list_node_t* head = list->head;
	void* head_value = head->value;

	list->head = head->next;
	if (list->size != 1) list->head->prev = NULL;
	else list->head = list->tail = NULL;

	free(head);
	list->size--;

	return head_value;
}

void* list_pop_back(list_t* list)
{
	if (list->size == 0) return NULL;

	list_node_t* tail = list->tail;
	void* tail_value = tail->value;

	list->tail = tail->prev;
	if (list->size != 1) list->tail->next = NULL;
	else list->head = list->tail = NULL;

	free(tail);
	list->size--;

	return tail_value;
}


void list_clear(list_t* list) { while (!list_is_empty(list)) list_pop_back(list); }
bool list_is_empty(const list_t* list) { return list->size == 0; }