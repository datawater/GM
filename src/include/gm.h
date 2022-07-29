#include <stdint.h>

#include "gasm.h"

#define GM_STACK_CAPASITY 255

typedef uint64_t Word;
typedef struct {
	Word stack[GM_STACK_CAPASITY];
	size_t stack_size;
} GM;

typedef enum {
	GM_STACK_SIZE_LIMIT_REACHED_ERR,
	GM_NOT_ENOUGH_ERR,
	GM_STACK_UNDERFLOW_ERR
} GM_ERROR;

typedef enum {
	GM_NOT_CLEAN_DIV_WARN
} GM_WARN;

void gm_evaluate_instruction(GM *gm, ASM_INSTRUCTION instruction);
void gm_error(GM_ERROR err);
void print_stack(const GM* gm);

#ifdef GM_IMPLEMENTATION

#include <stdlib.h>

void gm_error(GM_ERROR err) {
	switch (err) {
		case GM_STACK_SIZE_LIMIT_REACHED_ERR: printf("[PANIC] Reached the stack size limit\n"); exit(1);
		case GM_NOT_ENOUGH_ERR: 						 printf("[PANIC] Not enough items on the stack"); exit(1);
		case GM_STACK_UNDERFLOW_ERR: 				 printf("[PANIC] Stack underflow\n"); exit(1);
		default: {};
	}
}

void gm_warn(GM_WARN warn) {
	switch (warn) {
		case GM_NOT_CLEAN_DIV_WARN: printf("[WARNING] Number not cleanly divisible by other one");
	}
}

void gm_evaluate_instruction(GM *gm, const ASM_INSTRUCTION instruction) {
	switch (instruction.type) {
		case GASM_PUSH: {
			if (gm->stack_size == GM_STACK_CAPASITY) {
				gm_error(GM_STACK_SIZE_LIMIT_REACHED_ERR);
			}
			gm->stack[gm->stack_size] = instruction.arguments;
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
			gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] + instruction.arguments;
		} break;
		case GASM_SUB: {
			if ((int) gm->stack_size < 1) {
				gm_error(GM_NOT_ENOUGH_ERR);
			}
			gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] - instruction.arguments;
		} break;
		case GASM_MUL: {
			if ((int) gm->stack_size < 1) {
				gm_error(GM_NOT_ENOUGH_ERR);
			}
			gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] * instruction.arguments;
		} break;
		case GASM_DIV: {
			if ((int) gm->stack_size < 1) {gm_error(GM_NOT_ENOUGH_ERR);}
			if (gm->stack[gm->stack_size-1] % instruction.arguments != 0) {gm_warn(GM_NOT_CLEAN_DIV_WARN);}
			gm->stack[gm->stack_size-1] = (int64_t) (gm->stack[gm->stack_size-1] / instruction.arguments);
		} break;
		case GASM_BIT_L: {
			if ((int) gm->stack_size < 1) {
				gm_error(GM_NOT_ENOUGH_ERR);
			}
			gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] << instruction.arguments;
		} break;
		case GASM_BIT_R: {
			if ((int) gm->stack_size < 1) {
				gm_error(GM_NOT_ENOUGH_ERR);
			}
			gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] >> instruction.arguments;
		} break;
		default: {};
	}
}

void print_stack(const GM* gm) {
	printf("Stack:\n\t");
	for (int i = 0; i < (int) gm->stack_size; ++i) {
		i != (int) gm->stack_size-1 ? (void) printf("%ld,", gm->stack[i]) : printf("%ld", gm->stack[i]);
	}
	printf("\n");
}

#endif // GM_IMPLEMENTATION