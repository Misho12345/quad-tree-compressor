#include "tree.h"
#include "../queue/queue.h"

#include <stdlib.h>

tree_node_t* create_node(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
{
	tree_node_t* node = malloc(sizeof(tree_node_t));
	*node = (tree_node_t){ .x = x, .y = y, .width = width, .height = height, .color = 0x00000000 };
	node->children[0] = NULL;
	node->children[1] = NULL;
	node->children[2] = NULL;
	node->children[3] = NULL;

	return node;
}

void release_tree(tree_t* tree)
{
	queue_t* queue = queue_init();
	queue_push(queue, tree->root);
	while (!queue_is_empty(queue))
	{
		tree_node_t* node = queue_pop(queue);

		if (node->children[0])
		{
			queue_push(queue, node->children[0]);
			queue_push(queue, node->children[1]);
			queue_push(queue, node->children[2]);
			queue_push(queue, node->children[3]);
		}

		free(node);
	}

	free(tree);
}