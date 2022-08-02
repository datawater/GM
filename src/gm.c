#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
	#include "include/winuinistd.h"
#else
	#include <unistd.h>
#endif
#include <errno.h>

#include "version.h"

#define CONSOLE_IMPLEMENTATION
#include "include/console.h"

#define UTILS_IMPLEMENTATION
#include "include/utils.h"

#define ASM_IMPLEMENTATION
#include "include/gasm.h"

#define GM_IMPLEMENTATION
#include "include/gm.h"

int *PIPEDORNOT;

void usage() {
	char* author  = "datawater <datawater1@gmail.com>";
	char* version = malloc(sizeof(char)*6); snprintf(version, (size_t) 6, "%d.%d.%d", MAJOR_VERSION, MINOR_VERSION, SMALL_VERSION);
	if (*PIPEDORNOT) {
		printf("Author: %s\n",  author);
		printf("Version: %s\n", version);
		printf("Usage:\n");
	} else {
		color(FYELLOW); printf("Author: "); color(RESET);
		printf("%s\n", author);
		color(FYELLOW); printf("Version: "); color(RESET);
		printf("%s\n", version);
		color(FYELLOW); printf("Usage:\n"); color(RESET);
	}
	printf("\tgm <Input_file> <options>\n\t\tOptions:\n\t\t\t--help:\n\t\t\t\tDisplays This Help Message\n");
	free(version);
}

int main(int argc, char** argv) {
	PIPEDORNOT = malloc(sizeof(int));
	*PIPEDORNOT = !(isatty(STDOUT_FILENO));
	static GM gm = {0};

	if (argc < 2) {
		usage();
		!*PIPEDORNOT ? error("Input file not provided") : (void) printf("[ERROR] Input file not provided");
		exit(1);
	} else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1],"-h") == 0) {
		usage();
		exit(0);
	}

	if (!file_exist(argv[1])) {
		!*PIPEDORNOT ? error("Input file does not exist") : (void) printf("[ERROR] Input file does not exist");
		exit(1);
	}

	int8_t buffer[256] = {0};
	memset(buffer, -1, 256);
	size_t bytes_read;
	FILE* bmfile = fopen(argv[1], "rb");

	if (bmfile == NULL) {
		char* errmsg = malloc(sizeof(char) * 64); sprintf(errmsg,"Error opening a file: %s\n", strerror(errno));
		!*PIPEDORNOT ? error(errmsg) : (void) printf("%s",errmsg);
		free(errmsg);
		exit(1);
	}

	while ((bytes_read = fread(buffer, 1, 256, bmfile)) > 0) {
		int8_t type; char argument[8] = {0}; short j = -1;
		for (int i = 0; i < 256; ++i) {
			memset(argument, 0, 8);
			if (buffer[i] == 0) {type = -1; continue;} if (buffer[i] == -1) {break;}
			if (buffer[i+1] == 58) {
				type = buffer[i]; i++; 
				for (int n = 0; n < 256; ++n) {
					i++; if (buffer[i] == 0) {break;}
					argument[j++] = buffer[i];
				}
			} else if (buffer[i+1] == 0) {
				type = buffer[i];
			}
			gm.program[gm.program_size] = gasm_binary_instruction_to_instruction((BINARY_ASM_INSTRUCTION) {type, atoi(argument)});
			gm.program_size++;
			j = 0;
		}
	}
	gm_evaluate_program(&gm);
	free(PIPEDORNOT);
	return 0;
}