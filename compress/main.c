#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bmp_reader/reader.h"
#include "quad_tree_compress/compress.h"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Usage: %s ____.bmp ____.kon\n", argv[0]);
		return 1;
	}

	time_t start, end;
	time(&start);

	const image_t img = read_bmp(argv[1]);
	compress(argv[2], img);

	time(&end);
	printf("Time for compressing: %.2g seconds\n", difftime(end, start) / CLOCKS_PER_SEC);

	free(img.data);
	return 0;
}
