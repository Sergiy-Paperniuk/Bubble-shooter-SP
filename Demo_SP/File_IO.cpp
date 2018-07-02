#include "stdafx.h"
#include "File_IO.h"

#include <stdlib.h>  // malloc, free, rand
#include <stdio.h>  // FILE

void* get_file_contents(const char* filename, size_t* length)
{
	FILE *f;

	errno_t error = fopen_s(&f, filename, "r");

	void *buffer;

	if (!f)
	{
		fprintf(stdout, "Unable to open %s for reading\n", filename);
		return nullptr;
	}

	fseek(f, 0, SEEK_END);
	*length = ftell(f);
	fseek(f, 0, SEEK_SET);

	buffer = malloc(static_cast<size_t>(*length + 1));

	if (buffer == nullptr)
	{
		return nullptr;
	}

	*length = fread(buffer, 1, static_cast<size_t>(*length), f);
	fclose(f);
	((char*)buffer)[static_cast<size_t>(*length)] = '\0';

	return buffer;
}
