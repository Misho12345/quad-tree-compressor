#pragma once
#include <stdint.h>

typedef struct
{
	uint32_t width;
	uint32_t height;
	uint8_t color_byte_size;
	uint8_t* data;
} image_t;
