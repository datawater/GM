#pragma once

#include <stdint.h>

typedef enum {
	// TODO: implement new instructions
	/*
		Arethmetic operations: [x]
		bitshit operations:		  [x]
		Jump instruction:			  [x]
		variables:						  [  ]
		if statements:				  [x]
		halt instruction:			  [x]
		print instruction:			  [x]
		maybe linux syscalls:	  [ ? ]
	*/
	GASM_NOP,
	GASM_PUSH,
	GASM_POP,
	GASM_DUP,
	GASM_SWAP,
	GASM_OVER,
	GASM_CLS,
	GASM_ADD,
	GASM_SUB,
	GASM_MUL,
	GASM_DIV,
	GASM_BIT_L,
	GASM_BIT_R,
	GASM_JMP,
	GASM_HALT,
	GASM_PRINT,
	GASM_IF,
	GASM_ENDIF,
	GASM_EQU,
	GASM_MORE,
	GASM_LESS
} ASM_INSTRUCTION_TYPE;

typedef struct {
	ASM_INSTRUCTION_TYPE   type;
	uint64_t 								   arguments;
} ASM_INSTRUCTION;

typedef struct {
	uint8_t type;
	uint64_t arguments;
} BINARY_ASM_INSTRUCTION;

typedef struct {
	char* type;
	uint64_t arguments;
} TEXT_ASM_INSTRUCTION;

BINARY_ASM_INSTRUCTION gasm_instruction_to_binary_instruction(const ASM_INSTRUCTION instruction);
ASM_INSTRUCTION 			      gasm_text_instruction_to_instruction(const TEXT_ASM_INSTRUCTION instruction);
ASM_INSTRUCTION  			  gasm_binary_instruction_to_instruction(const BINARY_ASM_INSTRUCTION instruction);
void 										  gasm_write_binary_instruction(FILE* fd, const BINARY_ASM_INSTRUCTION instruction);

#ifdef ASM_IMPLEMENTATION

#include <string.h>
#include <inttypes.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wswitch-enum"

BINARY_ASM_INSTRUCTION gasm_instruction_to_binary_instruction(const ASM_INSTRUCTION instruction) {
	switch (instruction.type) {
		case GASM_NOP:         return (BINARY_ASM_INSTRUCTION) {0x01, -1};
		case GASM_PUSH:  	   return (BINARY_ASM_INSTRUCTION) {0x02, instruction.arguments};
		case GASM_POP:  		   return (BINARY_ASM_INSTRUCTION) {0x03, -1};
		case GASM_DUP:  		   return (BINARY_ASM_INSTRUCTION) {0x04, -1};
		case GASM_SWAP:  	   return (BINARY_ASM_INSTRUCTION) {0x05, -1};
		case GASM_OVER:		   return (BINARY_ASM_INSTRUCTION) {0x06, -1};
		case GASM_CLS:  	       return (BINARY_ASM_INSTRUCTION) {0x07, -1};
		case GASM_ADD:  	   return (BINARY_ASM_INSTRUCTION) {0x08, instruction.arguments};
		case GASM_SUB:  	       return (BINARY_ASM_INSTRUCTION) {0x09, instruction.arguments};
		case GASM_MUL:		   return (BINARY_ASM_INSTRUCTION) {0x0A, instruction.arguments};
		case GASM_DIV:		   return (BINARY_ASM_INSTRUCTION) {0x0B, instruction.arguments};
		case GASM_BIT_L:		   return (BINARY_ASM_INSTRUCTION) {0x0C, instruction.arguments};
		case GASM_BIT_R:		   return (BINARY_ASM_INSTRUCTION) {0x0D, instruction.arguments};
		case GASM_JMP:		   return (BINARY_ASM_INSTRUCTION) {0x0E, instruction.arguments};
		case GASM_HALT: 	   return (BINARY_ASM_INSTRUCTION) {0x0F, instruction.arguments};
		case GASM_PRINT:	   return (BINARY_ASM_INSTRUCTION) {0x10, instruction.arguments};
		case GASM_IF:			   return (BINARY_ASM_INSTRUCTION) {0x11, instruction.arguments};
		case GASM_ENDIF:	   return (BINARY_ASM_INSTRUCTION) {0x12, -1};
		case GASM_EQU:	   	   return (BINARY_ASM_INSTRUCTION) {0x13, -1};
		case GASM_MORE:	   return (BINARY_ASM_INSTRUCTION) {0x14, -1};
		case GASM_LESS:	   	   return (BINARY_ASM_INSTRUCTION) {0x15, -1};
		default: 						   return (BINARY_ASM_INSTRUCTION) {0xFF, -1};
	}
}
#pragma GCC diagnostic pop

ASM_INSTRUCTION gasm_text_instruction_to_instruction(const TEXT_ASM_INSTRUCTION instruction) {
	if (strcmp(instruction.type, "nop") == 0) {
		return (ASM_INSTRUCTION) {GASM_NOP, -1};
	} else if (strcmp(instruction.type, "push") == 0) {
		return (ASM_INSTRUCTION) {GASM_PUSH, instruction.arguments};
	} else if (strcmp(instruction.type, "pop") == 0) {
		return (ASM_INSTRUCTION) {GASM_POP, -1};
	} else if (strcmp(instruction.type, "dup") == 0) {
		return (ASM_INSTRUCTION) {GASM_DUP, -1};
	} else if (strcmp(instruction.type, "swap") == 0) {
		return (ASM_INSTRUCTION) {GASM_SWAP, -1};
	} else if (strcmp(instruction.type, "over") == 0) {
		return (ASM_INSTRUCTION) {GASM_OVER, -1};
	} else if (strcmp(instruction.type, "cls") == 0) {
		return (ASM_INSTRUCTION) {GASM_CLS, -1};
	} else if (strcmp(instruction.type, "add") == 0) {
		return (ASM_INSTRUCTION) {GASM_ADD, instruction.arguments};
	} else if (strcmp(instruction.type, "sub") == 0) {
		return (ASM_INSTRUCTION) {GASM_SUB, instruction.arguments};
	} else if (strcmp(instruction.type, "mul") == 0) {
		return (ASM_INSTRUCTION) {GASM_MUL, instruction.arguments};
	} else if (strcmp(instruction.type, "div") == 0) {
		return (ASM_INSTRUCTION) {GASM_DIV, instruction.arguments};
	} else if (strcmp(instruction.type, "bitl") == 0) {
		return (ASM_INSTRUCTION) {GASM_BIT_L, instruction.arguments};
	} else if (strcmp(instruction.type, "bitr") == 0) {
		return (ASM_INSTRUCTION) {GASM_BIT_R, instruction.arguments};
	} else if (strcmp(instruction.type, "jmp") == 0) {
		return (ASM_INSTRUCTION) {GASM_JMP, instruction.arguments};
	} else if (strcmp(instruction.type, "halt") == 0) {
		return (ASM_INSTRUCTION) {GASM_HALT, instruction.arguments};
	} else if (strcmp(instruction.type, "print") == 0) {
		return (ASM_INSTRUCTION) {GASM_PRINT, instruction.arguments};
	} else if (strcmp(instruction.type, "if") == 0) {
		return (ASM_INSTRUCTION) {GASM_IF, -1};
	} else if (strcmp(instruction.type, "endif") == 0) {
		return (ASM_INSTRUCTION) {GASM_ENDIF, -1};
	} else if (strcmp(instruction.type, "equ") == 0) {
		return (ASM_INSTRUCTION) {GASM_EQU, -1};
	} else if (strcmp(instruction.type, "more") == 0) {
		return (ASM_INSTRUCTION) {GASM_MORE, -1};
	} else if (strcmp(instruction.type, "less") == 0) {
		return (ASM_INSTRUCTION) {GASM_LESS, -1};
	} else {
		return (ASM_INSTRUCTION) {255, -1};
	}
}

char* instruction_type_to_string(const ASM_INSTRUCTION_TYPE a) {
	switch (a) {
		case GASM_NOP:         {char* t = malloc(sizeof(char) * 256); t = "GASM_NOP"; return t;}
		case GASM_PUSH:  	   {char* t = malloc(sizeof(char) * 256); t = "GASM_PUSH"; return t;}
		case GASM_POP:  		   {char* t = malloc(sizeof(char) * 256); t = "GASM_POP"; return t;}
		case GASM_DUP:  		   {char* t = malloc(sizeof(char) * 256); t = "GASM_DUP"; return t;}
		case GASM_SWAP:  	   {char* t = malloc(sizeof(char) * 256); t = "GASM_SWAP"; return t;}
		case GASM_OVER:		   {char* t = malloc(sizeof(char) * 256); t = "GASM_OVER"; return t;}
		case GASM_CLS:  	       {char* t = malloc(sizeof(char) * 256); t = "GASM_CLS"; return t;}
		case GASM_ADD:  	   {char* t = malloc(sizeof(char) * 256); t = "GASM_ADD"; return t;}
		case GASM_SUB:  	       {char* t = malloc(sizeof(char) * 256); t = "GASM_SUB"; return t;}
		case GASM_MUL:		   {char* t = malloc(sizeof(char) * 256); t = "GASM_MUL"; return t;}
		case GASM_DIV:		   {char* t = malloc(sizeof(char) * 256); t = "GASM_DIV"; return t;}
		case GASM_BIT_L:		   {char* t = malloc(sizeof(char) * 256); t = "GASM_BIT_L"; return t;}
		case GASM_BIT_R:		   {char* t = malloc(sizeof(char) * 256); t = "GASM_BIT_R"; return t;}
		case GASM_JMP:		   {char* t = malloc(sizeof(char) * 256); t = "GASM_JMP"; return t;}
		case GASM_HALT: 	   {char* t = malloc(sizeof(char) * 256); t = "GASM_HALT"; return t;}
		case GASM_PRINT:	   {char* t = malloc(sizeof(char) * 256); t = "GASM_PRINT"; return t;}
		case GASM_IF:			   {char* t = malloc(sizeof(char) * 256); t = "GASM_IF"; return t;}
		case GASM_ENDIF:	   {char* t = malloc(sizeof(char) * 256); t = "GASM_ENDIF"; return t;}
		case GASM_EQU:	   	   {char* t = malloc(sizeof(char) * 256); t = "GASM_EQU"; return t;}
		case GASM_MORE:	   {char* t = malloc(sizeof(char) * 256); t = "GASM_MORE"; return t;}
		case GASM_LESS:	   	   {char* t = malloc(sizeof(char) * 256); t = "GASM_LESS"; return t;}
		default: 						   {char* t = malloc(sizeof(char) * 256); t = "INVALID!"; return t;}
	}
}
 
ASM_INSTRUCTION  gasm_binary_instruction_to_instruction(const BINARY_ASM_INSTRUCTION instruction) {
	switch (instruction.type) {
		case 0x01:         return (ASM_INSTRUCTION) {GASM_NOP, -1};
		case 0x02:  	   return (ASM_INSTRUCTION) {GASM_PUSH, instruction.arguments};
		case 0x03:  	   return (ASM_INSTRUCTION) {GASM_POP, -1};
		case 0x04:  	   return (ASM_INSTRUCTION) {GASM_DUP, -1};
		case 0x05:  	   return (ASM_INSTRUCTION) {GASM_SWAP, -1};
		case 0x06:		   return (ASM_INSTRUCTION) {GASM_OVER, -1};
		case 0x07:  	   return (ASM_INSTRUCTION) {GASM_CLS, -1};
		case 0x08:  	   return (ASM_INSTRUCTION) {GASM_ADD, instruction.arguments};
		case 0x09:  	   return (ASM_INSTRUCTION) {GASM_SUB, instruction.arguments};
		case 0x0A:		   return (ASM_INSTRUCTION) {GASM_MUL, instruction.arguments};
		case 0x0B:		   return (ASM_INSTRUCTION) {GASM_DIV, instruction.arguments};
		case 0x0C: 	       return (ASM_INSTRUCTION) {GASM_BIT_L, instruction.arguments};
		case 0x0D:		   return (ASM_INSTRUCTION) {GASM_BIT_R, instruction.arguments};
		case 0x0E:		   return (ASM_INSTRUCTION) {GASM_JMP, instruction.arguments};
		case 0x0F:		   return (ASM_INSTRUCTION) {GASM_HALT, instruction.arguments};
		case 0x10:		   return (ASM_INSTRUCTION) {GASM_PRINT, instruction.arguments};
		case 0x11:		   return (ASM_INSTRUCTION) {GASM_IF, -1};
		case 0x12:		   return (ASM_INSTRUCTION) {GASM_ENDIF, -1};
		case 0x13:		   return (ASM_INSTRUCTION) {GASM_EQU, -1};
		case 0x14:		   return (ASM_INSTRUCTION) {GASM_MORE, -1};
		case 0x15:		   return (ASM_INSTRUCTION) {GASM_LESS, -1};
		default: 			   return (ASM_INSTRUCTION) {255, -1};
	}
}

void gasm_write_binary_instruction(FILE* fd, const BINARY_ASM_INSTRUCTION instruction) {
	uint8_t t = instruction.type;
	fprintf(fd, "%c", t); 
	t == 0x02 || t == 0x08 || t ==0x09 || t == 0x0A || t == 0x0B || t == 0x0C || t == 0x0D || t == 0x0E  || t == 0x0F || t == 0x10 ? fprintf(fd, ":%" PRIu64 "c%c", instruction.arguments, '\0') : fprintf(fd, "%c",'\0');
}

#endif // ASM_IMPLEMENTATION
