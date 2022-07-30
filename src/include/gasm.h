#pragma once

#include <stdint.h>

typedef enum {
	// TODO: implement new instructions such as: if (cmp)
	/*
		Arethmetic operations: [x]
		bitshit operations:		  [x]
		Jump instruction:			  [x]
		if statements:				  [  ]
		halt instruction:			  [  ]
		print instruction:			  [  ]
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
	GASM_JMP
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

#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wswitch-enum"

BINARY_ASM_INSTRUCTION gasm_instruction_to_binary_instruction(const ASM_INSTRUCTION instruction) {
	switch (instruction.type) {
		case GASM_NOP:         return (BINARY_ASM_INSTRUCTION) {0x01, -1}; // 01
		case GASM_PUSH:  	   return (BINARY_ASM_INSTRUCTION) {0x02, instruction.arguments}; // 02
		case GASM_POP:  		   return (BINARY_ASM_INSTRUCTION) {0x03, -1}; // 03
		case GASM_DUP:  		   return (BINARY_ASM_INSTRUCTION) {0x04, -1}; // 04
		case GASM_SWAP:  	   return (BINARY_ASM_INSTRUCTION) {0x05, -1}; // 05
		case GASM_OVER:		   return (BINARY_ASM_INSTRUCTION) {0x06, -1}; // 06
		case GASM_CLS:  	       return (BINARY_ASM_INSTRUCTION) {0x07, -1}; // 07
		case GASM_ADD:  	   return (BINARY_ASM_INSTRUCTION) {0x08, instruction.arguments}; // 08
		case GASM_SUB:  	       return (BINARY_ASM_INSTRUCTION) {0x09, instruction.arguments}; // 09
		case GASM_MUL:		   return (BINARY_ASM_INSTRUCTION) {0x0A, instruction.arguments}; // 10
		case GASM_DIV:		   return (BINARY_ASM_INSTRUCTION) {0x0B, instruction.arguments}; // 11
		case GASM_BIT_L:		   return (BINARY_ASM_INSTRUCTION) {0x0C, instruction.arguments}; // 12
		case GASM_BIT_R:		   return (BINARY_ASM_INSTRUCTION) {0x0D, instruction.arguments}; // 13
		case GASM_JMP:		   return (BINARY_ASM_INSTRUCTION) {0x0E, instruction.arguments}; // 14
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
	} else {
		return (ASM_INSTRUCTION) {255, -1};
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
		default: 			   return (ASM_INSTRUCTION) {255, -1};
	}
}

void gasm_write_binary_instruction(FILE* fd, const BINARY_ASM_INSTRUCTION instruction) {
	uint8_t t = instruction.type;
	fprintf(fd, "%c", t); 
	t == 0x02 || t == 0x08 || t ==0x09 || t == 0x0A || t == 0x0B || t == 0x0C || t == 0x0D || t == 0x0E  ? fprintf(fd, ":%ld%c", instruction.arguments, '\0') : fprintf(fd, "%c",'\0');
}

#endif // ASM_IMPLEMENTATION