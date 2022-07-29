#include <stdint.h>

#include "gasm.h"

#define GM_STACK_CAPASITY 255

typedef uint64_t Word;
typedef struct {
	Word stack[GM_STACK_CAPASITY];
	size_t stack_size;
} GM;

typedef enum {
	GM_NO_ERROR,
	GM_STACK_SIZE_LIMIT_REACHED,
	GM_STACK_UNDERFLOW
} GM_ERROR;

void gm_evaluate_instruction(GM *gm, ASM_INSTRUCTION instruction);
void gm_error(GM_ERROR err);
void print_stack(const GM* gm);

#ifdef GM_IMPLEMENTATION

#include <stdlib.h>
#include <assert.h>

#include "console.h"

void gm_error(GM_ERROR err) {
	switch (err) {
		case GM_STACK_SIZE_LIMIT_REACHED: printf("[ERROR] Reached the stack size limit\n"); exit(1);
		case GM_STACK_UNDERFLOW: printf("[ERROR] Stack underflow\n"); exit(1);
		default: {};
	}
}

void gm_evaluate_instruction(GM *gm, const ASM_INSTRUCTION instruction) {
	switch (instruction.type) {
		case GASM_PUSH: {
			if (gm->stack_size == GM_STACK_CAPASITY) {
				gm_error(GM_STACK_SIZE_LIMIT_REACHED);
			}
			gm->stack[gm->stack_size] = instruction.arguments;
			++(gm->stack_size);
		} break;
		case GASM_POP: {
			if ((int) gm->stack_size < 1) {
				gm_error(GM_STACK_UNDERFLOW);
			}
			gm->stack[gm->stack_size] = '\0';
			--(gm->stack_size);
		} break;
		case GASM_DUP: {
			if ((int) gm->stack_size < 1) {
				gm_error(GM_STACK_UNDERFLOW);
			} else if (gm->stack_size == GM_STACK_CAPASITY) {
				gm_error(GM_STACK_SIZE_LIMIT_REACHED);
			}
			gm->stack_size++;
			gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-2];
		} break;
		case GASM_SWAP: {
			if ((int) gm->stack_size < 2) {
				gm_error(GM_STACK_UNDERFLOW);
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
				gm_error(GM_STACK_UNDERFLOW);
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
			if ((int) gm->stack_size < 2) {
				gm_error(GM_STACK_UNDERFLOW);
			}
			// Stack 69 420 -> 489
			Word e2 = gm->stack[gm->stack_size-2];
			gm->stack[gm->stack_size-2] = gm->stack[gm->stack_size-1] + e2;
			gm->stack_size--;
		} break;
		case GASM_SUB: {
			if ((int) gm->stack_size < 2) {
				gm_error(GM_STACK_UNDERFLOW);
			}
			// Stack 69 489 -> 420
			Word e2 = gm->stack[gm->stack_size-2];
			gm->stack[gm->stack_size-2] = gm->stack[gm->stack_size-1] - e2;
			gm->stack_size--;
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