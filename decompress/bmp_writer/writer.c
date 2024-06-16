#include "writer.h"

#include <stdio.h>

void write_bmp(const char* filename, const image_t img)
{
	FILE* f = fopen(filename, "wb");

	if (!f)
	{
		printf("Failed to open %s\n", filename);
		return;
	}

	const uint32_t rounded_width = (img.width * img.color_byte_size + 3) & -4;
	const uint32_t image_size = rounded_width * img.height;

	const uint32_t zero = 0;
	const uint32_t offset = 54;
	const uint32_t file_size = offset + image_size;

	fwrite("BM", 1, 2, f);
	fwrite(&file_size, 4, 1, f);
	fwrite(&zero, 4, 1, f);
	fwrite(&offset, 4, 1, f);

	const uint32_t header_size = 40;
	const uint16_t planes = 1;
	const uint16_t bits_per_pixel = img.color_byte_size * 8;

	fwrite(&header_size, 4, 1, f);
	fwrite(&img.width, 4, 1, f);
	fwrite(&img.height, 4, 1, f);
	fwrite(&planes, 2, 1, f);
	fwrite(&bits_per_pixel, 2, 1, f);
	fwrite(&zero, 4, 1, f);
	fwrite(&image_size, 4, 1, f);
	fwrite(&zero, 4, 1, f);
	fwrite(&zero, 4, 1, f);
	fwrite(&zero, 4, 1, f);
	fwrite(&zero, 4, 1, f);

	fwrite(img.data, 1, image_size, f);

	fclose(f);
}
