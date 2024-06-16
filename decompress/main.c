#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "quad_tree_decompress/decompress.h"
#include "bmp_writer/writer.h"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Usage: %s ____.kon ____.bmp\n", argv[0]);
		return 1;
	}

	time_t start, end;
	time(&start);

	const image_t img = decompress(argv[1]);
	write_bmp(argv[2], img);

	time(&end);
	printf("Time for decompressing: %.2g seconds\n", difftime(end, start) / CLOCKS_PER_SEC);

	free(img.data);
	return 0;
}