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
	architecture();
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

void write_instruction(char* type, char* args, FILE* outputfp) {
	ASM_INSTRUCTION instruction = gasm_text_instruction_to_instruction((TEXT_ASM_INSTRUCTION) {type, atoi(args)});
	if (instruction.type == 255) {
		char* errmsg = malloc(sizeof(char) * 64); sprintf(errmsg,"Invalid Assembly instruction: %s\n", type);
		!*PIPEDORNOT ? error(errmsg) : (void) printf("%s",errmsg);
		free(errmsg);
		exit(1);
	}

	BINARY_ASM_INSTRUCTION binary_instruction = gasm_instruction_to_binary_instruction(instruction);
	gasm_write_binary_instruction(outputfp, binary_instruction);
}

void parse_file(char* input, char* output) {
	FILE* inputfp = fopen(input, "r"); FILE* outputfp = fopen(output, "wb");
	gasm_write_binary_instruction(outputfp, (BINARY_ASM_INSTRUCTION) {0x01, 0});

	if (inputfp == NULL || outputfp == NULL) {
		char* errmsg = malloc(sizeof(char) * 64); sprintf(errmsg,"Error opening a file: %s\n", strerror(errno));
		!*PIPEDORNOT ? error(errmsg) : (void) printf("%s",errmsg);
		free(errmsg);
		exit(1);
	}

	int len = 256; char* line = malloc(sizeof(char)* len); int line_num = 1; int q = 1;
	while (fgets(line, len, inputfp) && q != 0) {
		line = trim(line); 
		if (line[0] == '#') {continue;}
		char type[12]; 	memset(type, 0, 12); char *args = malloc(sizeof(char) * 256); memset(args, 0, 256); int i = 0;
		for (; i < (int) strlen(line); ++i) {
			if (line[i] == ' ') {break;}
			type[i] = line[i]; 
		}

		if (type[strlen(type)-1] == '\n') {type[strlen(type)-1] = '\0';}

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
				if (strcmp(type, "add") == 0 || strcmp(type, "sub") == 0 || strcmp(type, "mul") == 0 || strcmp(type, "div") == 0) {write_instruction(type, args,outputfp); continue;}
				char* errmsg = malloc(sizeof(char) * 64); sprintf(errmsg,"Argument is not on line: %i\n", line_num);
				!*PIPEDORNOT ? error(errmsg) : (void) printf("%s",errmsg);
				free(errmsg);
				exit(1);
			}

			if (strcmp(type, "push") != 0) {
				if (atoi(args) == 0 && args[0] != '0') {				
					char* errmsg = malloc(sizeof(char) * 80); sprintf(errmsg,"Argument is a string when it doesnt neet to be a string on line: %i\n", line_num);
					!*PIPEDORNOT ? error(errmsg) : (void) printf("%s",errmsg);
					free(errmsg);
					exit(1);
				}
				if (strstr(args, " ") || strstr(args, ",")) {
					char* errmsg = malloc(sizeof(char) * 80); sprintf(errmsg,"There are multiple arguments where there need to be one. line: %i\n", line_num);
					!*PIPEDORNOT ? error(errmsg) : (void) printf("%s",errmsg);
					free(errmsg);
					exit(1);
				}
			} else {
				if (strstr(args, "'")) {
					// printf("String!\n");
					ASM_INSTRUCTION instruction = gasm_text_instruction_to_instruction((TEXT_ASM_INSTRUCTION) {type, 3});
					BINARY_ASM_INSTRUCTION binary_instruction = gasm_instruction_to_binary_instruction(instruction);
					gasm_write_binary_instruction(outputfp, binary_instruction); // EOT
					for (int j = 1; j < (int) strlen(args); j++) {
						if (args[j] != '\'' && args[j-1] != '\\') {
							gasm_write_binary_instruction(outputfp, (BINARY_ASM_INSTRUCTION) {0x02, (int) args[j]});
						}
					}
					continue;
				} else if (strstr(args, " ") || strstr(args, ",")) {
					char s[64];
					for (int j = 0; j < (int) strlen(args); j++) {
						if (args[j] == ' ') {
							gasm_write_binary_instruction(outputfp, (BINARY_ASM_INSTRUCTION) {0x02, atoi(s)});
							memset(s, 0, 64); args += j+1; j = -1;
						}
						s[j] = args[j];
					}
					gasm_write_binary_instruction(outputfp, (BINARY_ASM_INSTRUCTION) {0x02, atoi(s)});
					continue;
				} else {
					if (strstr(args, " ") || strstr(args, ",")) {
						char* errmsg = malloc(sizeof(char) * 80); sprintf(errmsg,"There are multiple arguments where there need to be one. line: %i\n", line_num);
						!*PIPEDORNOT ? error(errmsg) : (void) printf("%s",errmsg);
						free(errmsg);
						exit(1);
					}
				}
			}
		}

		write_instruction(type,args,outputfp);

		//fflush(outputfp);
		line[0] = '\0';
		++line_num;
		//free(args);
	}
}

int main(int argc, char** argv) {
	PIPEDORNOT = malloc(sizeof(int));
	*PIPEDORNOT = !(isatty(STDOUT_FILENO));

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

	argc < 3 ? (!*PIPEDORNOT ? warn("Ouput file not provided, Using the default one") : (void) printf("[WARN] Output file not provided, Using the default one")) : (void) 0;

	char* output_file = !(argc < 3) ? argv[2] : strcat(strdup(argv[1]), ".out.gm"); output_file[strlen(output_file)] = '\0';

	parse_file(argv[1], output_file);

	free(PIPEDORNOT);
	return 0;
}
