#include <stdint.h>

#include "gasm.h"

#define GM_STACK_CAPASITY 255

typedef uint64_t Word;

typedef struct {
	Word stack[GM_STACK_CAPASITY];
	size_t stack_size;
	ASM_INSTRUCTION program[200000];
	size_t program_size;
} GM;

typedef enum {
	GM_STACK_SIZE_LIMIT_REACHED_ERR,
	GM_NOT_ENOUGH_ERR,
	GM_STACK_UNDERFLOW_ERR,
	GM_JUMP_ITSELF,
	GM_INVALID_PRINT_MODE
} GM_ERROR;

typedef enum {
	GM_NOT_CLEAN_DIV_WARN
} GM_WARN;

void gm_evaluate_program(GM *gm);
void gm_error(GM_ERROR err);
void print_stack(const GM* gm);
size_t size_tdup(size_t n);

#ifdef GM_IMPLEMENTATION

#include <stdlib.h>

void gm_error(GM_ERROR err) {
	switch (err) {
		case GM_STACK_SIZE_LIMIT_REACHED_ERR: printf("[PANIC] Reached the stack size limit\n"); exit(1);
		case GM_NOT_ENOUGH_ERR: 						 printf("[PANIC] Not enough items on the stack"); exit(1);
		case GM_STACK_UNDERFLOW_ERR: 				 printf("[PANIC] Stack underflow\n"); exit(1);
		case GM_JUMP_ITSELF:										 printf("[PANIC] Program can not jump to itself\n"); exit(1);
		case GM_INVALID_PRINT_MODE:					 printf("[PANIC] Invalid print mode"); exit(1);
		default: {};
	}
}

void gm_warn(GM_WARN warn) {
	switch (warn) {
		case GM_NOT_CLEAN_DIV_WARN: printf("[WARNING] Number not cleanly divisible by other one");
	}
}

void gm_evaluate_program(GM *gm) {
	const int prog_size = (int) gm->program_size;
	for (int i = 0; i < prog_size; i++) {	
		switch (gm->program[i].type) {
			case GASM_PUSH: {
				if (gm->stack_size == GM_STACK_CAPASITY) {
					gm_error(GM_STACK_SIZE_LIMIT_REACHED_ERR);
				}
				gm->stack[gm->stack_size] = gm->program[i].arguments;
				++(gm->stack_size);
			} break;
			case GASM_POP: {
				if ((int) gm->stack_size < 1) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				gm->stack[gm->stack_size] = '\0';
				--(gm->stack_size);
			} break;
			case GASM_DUP: {
				if ((int) gm->stack_size < 1) {
					gm_error(GM_NOT_ENOUGH_ERR);
				} else if (gm->stack_size == GM_STACK_CAPASITY) {
					gm_error(GM_STACK_SIZE_LIMIT_REACHED_ERR);
				}
				gm->stack_size++;
				gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-2];
			} break;
			case GASM_SWAP: {
				if ((int) gm->stack_size < 2) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				// Stack: 420 420 69 -> 420 69 420
				Word e1 = gm->stack[gm->stack_size-1];
				Word e2 = gm->stack[gm->stack_size-2];

				gm->stack[gm->stack_size-1] = e2;
				gm->stack[gm->stack_size-2] = e1;
			} break;
			case GASM_CLS: {
				gm->stack_size = 0;
			} break;
			case GASM_OVER: {
				if ((int) gm->stack_size < 3) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				// Stack: 1 420 69 -> 69 1 420
				Word e1 = gm->stack[gm->stack_size-1];
				Word e2 = gm->stack[gm->stack_size-2];
				Word e3 = gm->stack[gm->stack_size-3];

				gm->stack[gm->stack_size-1] = e2;
				gm->stack[gm->stack_size-2] = e3;
				gm->stack[gm->stack_size-3] = e1;
			} break;
			case GASM_ADD: {
				if ((int) gm->stack_size < 1) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] + gm->program[i].arguments;
			} break;
			case GASM_SUB: {
				if ((int) gm->stack_size < 1) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] - gm->program[i].arguments;
			} break;
			case GASM_MUL: {
				if ((int) gm->stack_size < 1) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] * gm->program[i].arguments;
			} break;
			case GASM_DIV: {
				if ((int) gm->stack_size < 1) {gm_error(GM_NOT_ENOUGH_ERR);}
				if (gm->stack[gm->stack_size-1] % gm->program[i].arguments != 0) {gm_warn(GM_NOT_CLEAN_DIV_WARN);}
				gm->stack[gm->stack_size-1] = (int64_t) (gm->stack[gm->stack_size-1] / gm->program[i].arguments);
			} break;
			case GASM_BIT_L: {
				if ((int) gm->stack_size < 1) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] << gm->program[i].arguments;
			} break;
			case GASM_BIT_R: {
				if ((int) gm->stack_size < 1) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] >> gm->program[i].arguments;
			} break;
			case GASM_JMP: {
				if (gm->program_size-1 == gm->program[i].arguments) {
					gm_error(GM_JUMP_ITSELF);
				}
				i = (gm->program[i].arguments)-1;
			} break;
			case GASM_HALT: {
				// printf("%lu",gm->program[i].arguments);
				exit(gm->program[i].arguments);
			} break;
			case GASM_PRINT: {
				if ((int) gm->stack_size < 1) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				if (gm->program[i].arguments == 10 || gm->program[i].arguments == 11) {
					int r[256]; int k = 0;
					while (gm->stack[0] == (uint64_t) 3) {
						r[k] = (int) gm->stack[gm->stack_size-1];
						gm->stack[gm->stack_size] = '\0';
						--(gm->stack_size); k++;
					}
					for(int t = 0; t<k+1; t++){
						printf("%c",r[k-t]);
					};
					if (gm->program[i].arguments == 11) {printf("\n");}
				} else if (gm->program[i].arguments == 12 || gm->program[i].arguments == 13) {
					printf("%li\n",gm->stack[gm->stack_size-1]);
					gm->stack[gm->stack_size] = '\0';
					--(gm->stack_size);
					if (gm->program[i].arguments == 13) {printf("\n");}
				} else {
					gm_error(GM_INVALID_PRINT_MODE);
				}
			} break;
			default: {};
		}
		print_stack(gm);
	}
}

void print_stack(const GM* gm) {
	printf("Stack:\t");
	for (int i = 0; i < (int) gm->stack_size; ++i) {
		i != (int) gm->stack_size-1 ? (void) printf("%ld,", gm->stack[i]) : (void) printf("%ld", gm->stack[i]);
	}
	printf("\n");
}

size_t size_tdup(size_t n) {
	return n;
}

#endif // GM_IMPLEMENTATION