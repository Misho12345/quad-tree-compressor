#include "compress.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../queue/queue.h"
#include "../../quad_tree/tree.h"

const long double THRESHOLD = 15;


static void calculate_average(tree_node_t* node, const image_t img)
{
	long double channels[4] = { 0 };
	const size_t top_left = node->x + node->y * img.width;

	for (size_t i = 0; i < node->height; i++)
		for (size_t j = 0; j < node->width; j++)
			for (uint8_t k = 0; k < img.color_byte_size; k++)
				channels[k] += img.data[(top_left + i * img.width + j) * img.color_byte_size + k];

	for (uint8_t i = 0; i < img.color_byte_size; i++)
	{
		channels[i] /= node->width * node->height;
		node->color |= (uint32_t)channels[i] << 8 * i;
	}
}

static long double color_dist(const uint32_t c1, const uint32_t c2)
{
	return sqrtl(
		pow((double)(c1 >> 24 & 0xff) - (double)(c2 >> 24 & 0xff), 2) +
		pow((double)(c1 >> 16 & 0xff) - (double)(c2 >> 16 & 0xff), 2) +
		pow((double)(c1 >> 8 & 0xff) - (double)(c2 >> 8 & 0xff), 2) +
		pow((double)(c1 & 0xff) - (double)(c2 & 0xff), 2)
	);
}


static tree_t* create_tree(const image_t img)
{
	tree_t* tree = malloc(sizeof(tree_t));
	*tree = (tree_t)
	{
		.root = create_node(0, 0, img.width, img.height),
		.width = img.width,
		.height = img.height,
		.color_byte_size = img.color_byte_size
	};

	queue_t* queue = queue_init();
	queue_push(queue, tree->root);
	calculate_average(tree->root, img);

	while (!queue_is_empty(queue))
	{
		tree_node_t* node = queue_pop(queue);

		const uint32_t hw = node->width / 2;
		const uint32_t hh = node->height / 2;

		if (hw == 0 || hh == 0) continue;

		const size_t top_left = node->x + node->y * img.width;

		for (size_t i = 0; i < node->height; i++)
		{
			for (size_t j = 0; j < node->width; j++)
			{
				uint32_t color = 0;
				for (uint8_t k = 0; k < img.color_byte_size; k++)
					color |= img.data[(top_left + i * img.width + j) * img.color_byte_size + k] << 8 * k;

				if (color_dist(color, node->color) > THRESHOLD)
					goto generate_children;
			}
		}

		continue;

		generate_children:
		for (uint8_t i = 0; i < 4; i++)
		{
			const uint32_t x = node->x + (i % 2 ? hw : 0);
			const uint32_t y = node->y + (i / 2 ? hh : 0);
			const uint32_t w = i % 2 ? node->width  - hw : hw;
			const uint32_t h = i / 2 ? node->height - hh : hh;

			node->children[i] = create_node(x, y, w, h);
			calculate_average(node->children[i], img);
			queue_push(queue, node->children[i]);
		}
	}

	queue_release(queue);

	return tree;
}

void compress(const char* out_file, const image_t img)
{
	FILE* f = fopen(out_file, "wb");

	if (!f)
	{
		printf("Failed to open %s\n", out_file);
		return;
	}

	tree_t* tree = create_tree(img);
	fwrite(&tree->width, 4, 1, f);
	fwrite(&tree->height, 4, 1, f);
	fwrite(&tree->color_byte_size, 1, 1, f);

	queue_t* temp = queue_init();
	queue_push(temp, tree->root);

	while (!queue_is_empty(temp))
	{
		const tree_node_t* node = queue_pop(temp);

		if (node->children[0])
		{
			fputc('N', f);
			for (uint8_t i = 0; i < 4; i++)
				queue_push(temp, node->children[i]);
		}
		else
		{
			fputc('L', f);
			fwrite(&node->color, img.color_byte_size, 1, f);
		}
	}

	queue_release(temp);
	release_tree(tree);

	fclose(f);
}
