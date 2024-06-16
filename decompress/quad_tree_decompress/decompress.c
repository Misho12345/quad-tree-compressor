#include "decompress.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../quad_tree/tree.h"
#include "../../queue/queue.h"

static tree_t* extract_tree(FILE* f)
{
    tree_t* tree = malloc(sizeof(tree_t));

    fread(&tree->width, 4, 1, f);
    fread(&tree->height, 4, 1, f);
    fread(&tree->color_byte_size, 1, 1, f);

	tree->root = create_node(0, 0, tree->width, tree->height);

    queue_t* queue = queue_init();

    char type = fgetc(f);
    if (type == 'L')
    {
        fread(&tree->root->color, tree->color_byte_size, 1, f);
        return tree;
    }
    
	queue_push(queue, tree->root);

    while (!queue_is_empty(queue))
    {
	    tree_node_t* node = queue_pop(queue);

        const uint32_t hw = node->width / 2;
        const uint32_t hh = node->height / 2;

        for (uint8_t i = 0; i < 4; i++)
        {
            const uint32_t x = node->x + (i % 2 ? hw : 0);
            const uint32_t y = node->y + (i / 2 ? hh : 0);
            const uint32_t w = i % 2 ? node->width  - hw : hw;
            const uint32_t h = i / 2 ? node->height - hh : hh;

            type = fgetc(f);

            if (type == 'L')
            {
                tree_node_t* child = create_node(x, y, w, h);
                fread(&child->color, tree->color_byte_size, 1, f);
                node->children[i] = child;
            }
            else if (type == 'N')
            {
                tree_node_t* child = create_node(x, y, w, h);
                node->children[i] = child;
                queue_push(queue, child);
            }
        }
    }

    queue_release(queue);
    return tree;
}


static void fill_region(const image_t img, const tree_node_t* node)
{
    for (uint32_t i = node->y; i < node->y + node->height; i++)
        for (uint32_t j = node->x; j < node->x + node->width; j++)
            memcpy(&img.data[(i * img.width + j) * img.color_byte_size], &node->color, img.color_byte_size);
}

static image_t get_image_data(const tree_t* tree)
{
    const uint32_t rounded_width = (tree->root->width * tree->color_byte_size + 3) & -4;

    const image_t img = {
    	.width = tree->width,
    	.height = tree->height,
    	.color_byte_size = tree->color_byte_size,
    	.data = malloc(rounded_width * tree->root->height)
    };

    memset(img.data, 0, rounded_width * tree->root->height);

    queue_t* queue = queue_init();
    queue_push(queue, tree->root);

    while (!queue_is_empty(queue))
	{
		const tree_node_t* node = queue_pop(queue);

        if (node->children[0])
        {
            for (uint8_t i = 0; i < 4; i++)
        		queue_push(queue, node->children[i]);
        }
        else fill_region(img, node);
	}
    
    queue_release(queue);
	return img;
}


image_t decompress(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (!f)
	{
		printf("Failed to open %s\n", filename);
        return (image_t) { 0, 0, 0, NULL };
	}

	tree_t* tree = extract_tree(f);
	const image_t img = get_image_data(tree);

	release_tree(tree);
	fclose(f);

	return img;
}