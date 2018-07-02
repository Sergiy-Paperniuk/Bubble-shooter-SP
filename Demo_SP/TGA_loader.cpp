#include "stdafx.h"

// TGA image file loader
// Supports only uncompressed 24 bit TGA (RGB) and 32 bit TGA (RGBA)

#include <stdlib.h>  // malloc, free, rand
#include <stdio.h>  // FILE

#include "TGA_loader.h"

short le_short(unsigned char *bytes)
{
	return bytes[0] | ((char)bytes[1] << 8);
}

void* read_tga(const char *filename, int *w, int *h)
{
	struct tga_header
	{
		unsigned char  id_length;
		char  color_map_type;
		char  data_type_code;
		unsigned char  color_map_origin[2];
		unsigned char  color_map_length[2];
		char  color_map_depth;
		unsigned char  x_origin[2];
		unsigned char  y_origin[2];
		unsigned char  width[2];
		unsigned char  height[2];
		char  bits_per_pixel;
		char  image_descriptor;
	} header;

	size_t i, color_map_size, pixels_size;
	FILE *f;
	size_t read;
	void *pixels;

	errno_t error = fopen_s(&f, filename, "rb");

	if (!f)
	{
		fprintf(stdout, "Unable to open %s for reading\n", filename);
		return nullptr;
	}

	read = fread(&header, 1, sizeof(header), f);

	if (read != sizeof(header))
	{
		fprintf(stdout, "%s has incomplete tga header\n", filename);
		fclose(f);
		return nullptr;
	}
	if (header.data_type_code != 2)
	{
		fprintf(stdout, "%s is not an uncompressed RGB tga file\n", filename);
		fclose(f);
		return nullptr;
	}
	if (header.bits_per_pixel != 24 && header.bits_per_pixel != 32)
	{
		fprintf(stdout, "%s is not a 24-bit or 32-bit uncompressed RGB tga file\n", filename);
		fclose(f);
		return nullptr;
	}

	for (i = 0; i < header.id_length; ++i)
	{
		if (getc(f) == EOF)
		{
			fprintf(stdout, "%s has incomplete id string\n", filename);
			fclose(f);
			return nullptr;
		}
	}

	color_map_size = le_short(header.color_map_length) * (header.color_map_depth / 8);
	for (i = 0; i < color_map_size; ++i)
	{
		if (getc(f) == EOF)
		{
			fprintf(stdout, "%s has incomplete color map\n", filename);
			fclose(f);
			return nullptr;
		}
	}

	*w = le_short(header.width); *h = le_short(header.height);
	pixels_size = static_cast<size_t>(*w * *h * (header.bits_per_pixel / 8));
	pixels = malloc(pixels_size);

	if (pixels == nullptr)
	{
		return nullptr;
	}

	read = fread(pixels, 1, pixels_size, f);
	fclose(f);

	if (read != pixels_size)
	{
		fprintf(stdout, "%s has incomplete image\n", filename);
		free(pixels);

		return nullptr;
	}

	return pixels;
}
