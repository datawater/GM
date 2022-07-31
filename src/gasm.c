#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "version.h"

#define CONSOLE_IMPLEMENTATION
#include "include/console.h"

#define UTILS_IMPLEMENTATION
#include "include/utils.h"

#define ASM_IMPLEMENTATION
#include "include/gasm.h"

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
	printf("\tgasm <Input_file> <output_file> <options>\n\t\tOptions:\n\t\t\t--help:\n\t\t\t\tDisplays This Help Message\n");
	free(version);
}

void parse_file(char* input, char* output) {
	FILE* inputfp = fopen(input, "r"); FILE* outputfp = fopen(output, "wb");

	if (inputfp == NULL || outputfp == NULL) {
		char* errmsg = malloc(sizeof(char) * 64); sprintf(errmsg,"Error opening a file: %s\n", strerror(errno));
		!*PIPEDORNOT ? error(errmsg) : (void) printf(errmsg);
		free(errmsg);
		exit(1);
	}

	int len = 256; char* line = malloc(sizeof(char)* len); int line_num = 1; int q = 1;
	while (fgets(line, len, inputfp) && q != 0) {
		line = trim(line); 
		if (line[0] == '#') {continue;}
		char type[12]; 	memset(type, 0, 12); char args[256]; memset(args, 0, 256); int i = 0;
		for (; i < (int) strlen(line); ++i) {
			if (line[i] == ' ') {break;}
			type[i] = line[i]; 
		}

		if (strcmp(type, "halt") == 0) {
			q = 0;
		}

		line += i; line = trim(line);  
		if (0 == (int) strlen(line)) {memset(args, 0, 256);} 
		else {
			for (int i = 0; i < (int) strlen(line); ++i) {
				if (line[i] == '\n' || line[i] == '#') {break;} 
				args[i] = line[i]; 
			}
		}
		if (isspace(args[strlen(args)-1])) {args[strlen(args)-1] = '\0';}

		if (strcmp(type, "push") == 0 || strcmp(type, "add") == 0 || strcmp(type, "sub") == 0 || strcmp(type, "mul") == 0 || strcmp(type, "div") == 0 || strcmp(type, "bitl") == 0 || strcmp(type, "bitr") == 0 || strcmp(type, "jmp") == 0 || strcmp(type, "add") == 0 || strcmp(type, "halt") == 0) {
			if (strlen(args) == 0) {
				char* errmsg = malloc(sizeof(char) * 64); sprintf(errmsg,"Argument is not on line: %i\n", line_num);
				!*PIPEDORNOT ? error(errmsg) : (void) printf(errmsg);
				free(errmsg);
				exit(1);
			}

			if (strcmp(type, "push") != 0) {
				if (atoi(args) == 0 && args[0] != '0') {				
					char* errmsg = malloc(sizeof(char) * 80); sprintf(errmsg,"Argument is a string when it doesnt neet to be a string on line: %i\n", line_num);
					!*PIPEDORNOT ? error(errmsg) : (void) printf(errmsg);
					free(errmsg);
					exit(1);
				}
				if (strlen(strstr(args, " ")) > 0 || strlen(strstr(args, ",")) > 0) {
					char* errmsg = malloc(sizeof(char) * 80); sprintf(errmsg,"There are multiple arguments where there need to be one. line: %i\n", line_num);
					!*PIPEDORNOT ? error(errmsg) : (void) printf(errmsg);
					free(errmsg);
					exit(1);
				}
			}
		}

		ASM_INSTRUCTION instruction = gasm_text_instruction_to_instruction((TEXT_ASM_INSTRUCTION) {type, atoi(args)});
		if (instruction.type == 255) {
			char* errmsg = malloc(sizeof(char) * 64); sprintf(errmsg,"Invalid Assembly instruction: %s\n", type);
			!*PIPEDORNOT ? error(errmsg) : (void) printf(errmsg);
			free(errmsg);
			exit(1);
		}

		BINARY_ASM_INSTRUCTION binary_instruction = gasm_instruction_to_binary_instruction(instruction);
		gasm_write_binary_instruction(outputfp, binary_instruction);
		//fflush(outputfp);
		line[0] = '\0';
		++line_num;
	}
}

int main(int argc, char** argv) {
	PIPEDORNOT = malloc(sizeof(int));
	*PIPEDORNOT = !(isatty(STDOUT_FILENO));

	if (argc < 2) {
		usage();
		!*PIPEDORNOT ? error("Input file not provided") : printf("[ERROR] Input file not provided");
		exit(1);
	} else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1],"-h") == 0) {
		usage();
		exit(0);
	}

	if (!file_exist(argv[1])) {
		!*PIPEDORNOT ? error("Input file does not exist") : printf("[ERROR] Input file does not exist");
		exit(1);
	}

	argc < 3 ? (!*PIPEDORNOT ? warn("Ouput file not provided, Using the default one") : printf("[WARN] Output file not provided, Using the default one")) : (void) 0;

	char* output_file = !(argc < 3) ? argv[2] : strcat(strdup(argv[1]), ".out.gm"); output_file[strlen(output_file)] = '\0';

	parse_file(argv[1], output_file);

	free(PIPEDORNOT);
	return 0;
}
