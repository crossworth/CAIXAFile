#ifndef CAIXAFILE_H_
#define CAIXAFILE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VERSION  "1.0.0"
#define FILE_HEADER_TAG "CAIXA_FILE"

#define MAX_FILE_NAME 255
#define MAX_DESCRIPTION_SIZE 2048

typedef char byte;

typedef struct {
    char name[MAX_FILE_NAME];
    uint64_t size;
    byte *data;
} caixa_entry;


typedef struct {
    char name[MAX_FILE_NAME];
    char version[10];
    char description[MAX_DESCRIPTION_SIZE];
    uint32_t number_files;
    uint64_t created_on;
    uint64_t updated_on;
} caixa_header;


typedef struct {
	caixa_header header;
	caixa_entry **files;
} caixa_file;


void caixa_create_header(caixa_header *header, char *name, char *description) {
	strncpy(header->name, name, MAX_FILE_NAME);
	strncpy(header->version, VERSION, strlen(VERSION));
	strncpy(header->description, description, MAX_DESCRIPTION_SIZE);

	if (!header->number_files) {
		header->number_files = 0;
	}

	if (!header->created_on) {
		header->created_on = (uint64_t) time(0);
	}

	header->updated_on = (uint64_t) time(0);
}

int caixa_crete_entry_from_file(caixa_entry *entry, char *file_name) {
	strncpy(entry->name, file_name, MAX_FILE_NAME);

	FILE *file;

	file = fopen(file_name, "rb");

	if (!file) {
		fprintf(stderr, "Error while opening the file: %s\n", file_name);
		return(0);
	}

	fseek(file, 0, SEEK_END);
	entry->size = ftell(file);
	fseek(file, 0, SEEK_SET);

	entry->data = (byte*) malloc(entry->size);

	fread(entry->data, 1, entry->size, file);
	fclose(file);

	entry->data[entry->size] = '\0';

	return(1);
}

void caixa_add_entry(caixa_file *caixa, caixa_entry *entry) {
	if (!caixa->files || caixa->header.number_files == 0) {
		caixa->files = malloc(sizeof(caixa_entry));
	} else {
		caixa->files = realloc(caixa->files, sizeof(caixa_entry) * caixa->header.number_files + 1);
	}

	caixa->files[caixa->header.number_files] = malloc(entry->size + sizeof(*entry));
	memset(caixa->files[caixa->header.number_files], 0, entry->size + sizeof(*entry));

	memcpy(caixa->files[caixa->header.number_files], entry, sizeof(*entry));
	memcpy(caixa->files[caixa->header.number_files]->data, entry->data, entry->size);

	caixa->header.number_files++;
}

void caixa_free(caixa_file *caixa) {
	for(uint32_t index = 0; index < caixa->header.number_files; index++) {
		if (caixa->files) {

			if (caixa->files[index]) {

				if (caixa->files[index]->data) {
					free(caixa->files[index]->data);
					caixa->files[index]->data = 0;
				}

				free(caixa->files[index]);
				caixa->files[index] = 0;
			}

			free(caixa->files);
			caixa->files = 0;
		}
	
	}

	caixa->header.number_files = 0;
}

int caixa_write_to_file(caixa_file *caixa, char *file_name) {
	if (!caixa) {
		perror("The caixa pointer is invalid");
		return(0);
	}

	FILE *file;
	file = fopen(file_name, "wb");

	if (!file) {
		fprintf(stderr, "Could not write to the file %s\n", file_name);
		return(0);
	}

	caixa->header.updated_on = (uint64_t) time(0);

	fwrite(caixa->header.name, 1, MAX_FILE_NAME, file);
	fwrite(caixa->header.version, 1, 10, file);
	fwrite(caixa->header.description, 1, MAX_DESCRIPTION_SIZE, file);
	fwrite((void*)&caixa->header.number_files, 1, sizeof(uint32_t), file);
	fwrite((void*)&caixa->header.created_on, 1, sizeof(uint64_t), file);
	fwrite((void*)&caixa->header.updated_on, 1, sizeof(uint64_t), file);

	for(uint32_t index = 0; index < caixa->header.number_files; index++) {
		fwrite((void*)caixa->files[index], sizeof(caixa_entry), 1, file);
		fwrite((void*)caixa->files[index]->data, caixa->files[index]->size, 1, file);
	}

	fclose(file);
	return(1);
}

int caixa_open_file(caixa_file *caixa, char *file_name) {
	memset(caixa, 0, sizeof(caixa_file));

	FILE *file;

	file = fopen(file_name, "rb");

	if (!file) {
		fprintf(stderr, "Could not open the file %s\n", file_name);
		return(0);
	}

	fread(&caixa->header, 1, sizeof(caixa_header), file);

	caixa->files = malloc(1);

	for(uint32_t index = 0; index < caixa->header.number_files; index++) {
		caixa_entry buffer = {};

		fread((void*)&buffer, 1, sizeof(caixa_entry), file);

		buffer.data = (byte*)malloc(buffer.size);

		caixa->files = realloc(caixa->files, sizeof(caixa_entry) * (index + 1));
		caixa->files[index] = malloc(buffer.size + sizeof(caixa_entry));
		memset(caixa->files[index], 0, buffer.size + sizeof(caixa_entry));

		memcpy(caixa->files[index], &buffer, sizeof(caixa_entry));

		caixa->files[index]->data = malloc(buffer.size);

		fread(caixa->files[index]->data, 1, buffer.size, file);

		caixa->files[index]->data[buffer.size] = 0;
	}

	fclose(file);
	return(1);
}


byte* caixa_get_file(caixa_file *caixa, char *file_name) {
	for(uint32_t index = 0; index < caixa->header.number_files; index++) {
		if (strcmp(caixa->files[index]->name, file_name) == 0) {
			return caixa->files[index]->data;
		}
	}

	return(0);
}

int caixa_remove_file(caixa_file *caixa, char *file_name) {
	int32_t offset_file = -1;

	for(uint32_t index = 0; index < caixa->header.number_files; index++) {
		if (strcmp(caixa->files[index]->name, file_name) == 0) {
			offset_file = index;
			break;
		}
	}

	if (offset_file == -1) {
		return(0);
	}

	for(uint32_t index = offset_file; index < caixa->header.number_files - 1; index++) {
		strncpy(caixa->files[index]->name, caixa->files[index + 1]->name, MAX_FILE_NAME);
		caixa->files[index]->size = caixa->files[index + 1]->size;

		free(caixa->files[index]->data);
		caixa->files[index]->data = malloc(caixa->files[index]->size);
		memset(caixa->files[index]->data, 0, caixa->files[index]->size);
		memcpy(caixa->files[index]->data, caixa->files[index + 1]->data, caixa->files[index]->size);
	}

	free(caixa->files[caixa->header.number_files - 1]->data);
	free(caixa->files[caixa->header.number_files - 1]);
	caixa->header.number_files--;
	return(1);
}

#endif // CAIXAFILE_H_
