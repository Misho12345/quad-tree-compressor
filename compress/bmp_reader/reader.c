#include "reader.h"
#include <stdio.h>
#include <stdlib.h>

image_t read_bmp(const char* filename)
{
	image_t img = { 0, 0, 0, NULL };

	FILE* f = fopen(filename, "rb");
	if (!f)
	{
		printf("Could not open file %s\n", filename);
		return img;
	}

	char header[2];
	fread(header, 2, 1, f);
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Please select a valid .bmp file\n");
		fclose(f);
		return img;
	}

	uint32_t data_offset;
	fseek(f, 8, SEEK_CUR);
	fread(&data_offset, 4, 1, f);

	fseek(f, 4, SEEK_CUR);
	fread(&img.width, 4, 1, f);
	fread(&img.height, 4, 1, f);

	fseek(f, 2, SEEK_CUR);
	uint16_t color_bit_size;
	fread(&color_bit_size, 2, 1, f);
	img.color_byte_size = color_bit_size / 8;

	img.data = (uint8_t*)malloc(img.width * img.height * img.color_byte_size);
	fseek(f, data_offset, SEEK_SET);
	fread(img.data, 1, img.width * img.height * img.color_byte_size, f);

	fclose(f);
	return img;
}
