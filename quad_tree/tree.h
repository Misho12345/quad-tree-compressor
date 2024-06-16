#pragma once
#include <stdint.h>

typedef struct tree_node_t
{
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
	struct tree_node_t* children[4];
	uint32_t color;
} tree_node_t;

typedef struct
{
	tree_node_t* root;
	uint32_t width;
	uint32_t height;
	uint8_t color_byte_size;
} tree_t;

tree_node_t* create_node(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
void release_tree(tree_t* tree);