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
	GM_STACK_OVERFLOW_ERR,
	GM_STACK_UNDERFLOW_ERR,
	GM_NOT_ENOUGH_ERR,
	GM_JUMP_ITSELF_ERR,
	GM_INVALID_PRINT_MODE_ERR,
	GM_INT_OVERFLOW_ERR,
	GM_INT_UNDERFLOW_ERR
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

#define UTILS_IMPLEMENTATION
#include "utils.h"

void gm_error(GM_ERROR err) {
	switch (err) {
		case GM_STACK_OVERFLOW_ERR: 					 printf("[PANIC] Stack overflow\n"); exit(1);
		case GM_STACK_UNDERFLOW_ERR: 				 printf("[PANIC] Stack underflow\n"); exit(1);
		case GM_NOT_ENOUGH_ERR: 						 printf("[PANIC] Not enough items on the stack"); exit(1);
		case GM_JUMP_ITSELF_ERR:							 printf("[PANIC] Program can not jump to itself\n"); exit(1);
		case GM_INVALID_PRINT_MODE_ERR:			 printf("[PANIC] Invalid print mode"); exit(1);
		case GM_INT_OVERFLOW_ERR:						 printf("[PANIC] Int overflow\n"); exit(1);
		case GM_INT_UNDERFLOW_ERR:					 printf("[PANIC] Int undeflow\n"); exit(1);
		default: {};
	}
}

void gm_warn(GM_WARN warn) {
	switch (warn) {
		case GM_NOT_CLEAN_DIV_WARN: printf("[WARNING] Number not cleanly divisible by other one");
	}
}

int check_xflow(int64_t a, int64_t b, int64_t s) {
	if (a > 0 && b > 0 && s < 0) {
		return 1; // INT OVERFLOW
	} if (a < 0 && b < 0 && s > 0) {
		return 2; // INT UNDERFLOW
	}
	return 0;
}

void gm_evaluate_program(GM *gm) {
	const int prog_size = (int) gm->program_size;
	for (int i = 0; i < prog_size; i++) {	
		switch (gm->program[i].type) {
			case GASM_PUSH: {
				if (gm->stack_size == GM_STACK_CAPASITY) {
					gm_error(GM_STACK_OVERFLOW_ERR);
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
					gm_error(GM_STACK_OVERFLOW_ERR);
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
				if (!(strlen(itoa(gm->program[i].arguments)) > 0 && gm->program[i].arguments != 0)) {
					if ((int) gm->stack_size < 2) { 
						gm_error(GM_NOT_ENOUGH_ERR);
					}
					int p = gm->stack[gm->stack_size-1] + gm->stack[gm->stack_size-2];
					if (check_xflow(gm->stack[gm->stack_size-1], gm->stack[gm->stack_size-2], p) == 1) {
						gm_error(GM_INT_OVERFLOW_ERR);
					} else if (check_xflow(gm->stack[gm->stack_size-1], gm->stack[gm->stack_size-2], p) == 2) {
						gm_error(GM_INT_UNDERFLOW_ERR);
					}
					gm->stack_size++;
					gm->stack[gm->stack_size-1] = p;
				} else {
					if ((int) gm->stack_size < 1) { 
						gm_error(GM_NOT_ENOUGH_ERR);
					}
					if (check_xflow(gm->stack[gm->stack_size-1], gm->program[i].arguments, gm->stack[gm->stack_size-1] + gm->program[i].arguments) == 1) {
						gm_error(GM_INT_OVERFLOW_ERR);
					} else if (check_xflow(gm->stack[gm->stack_size-1], gm->program[i].arguments, gm->stack[gm->stack_size-1] + gm->program[i].arguments) == 2) {
						gm_error(GM_INT_UNDERFLOW_ERR);
					}
					gm->stack_size++;
					gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] + gm->program[i].arguments;
				}
			} break;
			case GASM_SUB: {
				if (!(strlen(itoa(gm->program[i].arguments)) > 0 && gm->program[i].arguments != 0)) {
					if ((int) gm->stack_size < 2) { 
						gm_error(GM_NOT_ENOUGH_ERR);
					}
					int p = gm->stack[gm->stack_size-1] - gm->stack[gm->stack_size-2];
					if (check_xflow(gm->stack[gm->stack_size-1], gm->stack[gm->stack_size-2], p) == 1) {
						gm_error(GM_INT_OVERFLOW_ERR);
					} else if (check_xflow(gm->stack[gm->stack_size-1], gm->stack[gm->stack_size-2], p) == 2) {
						gm_error(GM_INT_UNDERFLOW_ERR);
					}
					gm->stack_size++;
					gm->stack[gm->stack_size-1] = p;
				} else {
					if ((int) gm->stack_size < 1) { 
						gm_error(GM_NOT_ENOUGH_ERR);
					}
					if (check_xflow(gm->stack[gm->stack_size-1], gm->program[i].arguments, gm->stack[gm->stack_size-1] - gm->program[i].arguments) == 1) {
						gm_error(GM_INT_OVERFLOW_ERR);
					} else if (check_xflow(gm->stack[gm->stack_size-1], gm->program[i].arguments, gm->stack[gm->stack_size-1] - gm->program[i].arguments) == 2) {
						gm_error(GM_INT_UNDERFLOW_ERR);
					}
					gm->stack_size++;
					gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] - gm->program[i].arguments;
				}
			} break;
			case GASM_MUL: {
				if (!(strlen(itoa(gm->program[i].arguments)) > 0 && gm->program[i].arguments != 0)) {
					if ((int) gm->stack_size < 2) { 
						gm_error(GM_NOT_ENOUGH_ERR);
					}
					int p = gm->stack[gm->stack_size-1] * gm->stack[gm->stack_size-2];
					if (check_xflow(gm->stack[gm->stack_size-1], gm->stack[gm->stack_size-2], p) == 1) {
						gm_error(GM_INT_OVERFLOW_ERR);
					} else if (check_xflow(gm->stack[gm->stack_size-1], gm->stack[gm->stack_size-2], p) == 2) {
						gm_error(GM_INT_UNDERFLOW_ERR);
					}
					gm->stack_size++;
					gm->stack[gm->stack_size-1] = p;
				} else {
					if ((int) gm->stack_size < 1) { 
						gm_error(GM_NOT_ENOUGH_ERR);
					}
					if (check_xflow(gm->stack[gm->stack_size-1], gm->program[i].arguments, gm->stack[gm->stack_size-1] * gm->program[i].arguments) == 1) {
						gm_error(GM_INT_OVERFLOW_ERR);
					} else if (check_xflow(gm->stack[gm->stack_size-1], gm->program[i].arguments, gm->stack[gm->stack_size-1] * gm->program[i].arguments) == 2) {
						gm_error(GM_INT_UNDERFLOW_ERR);
					}
					gm->stack_size++;
					gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] * gm->program[i].arguments;
				}
			} break;
			case GASM_DIV: {
				if (!(strlen(itoa(gm->program[i].arguments)) > 0 && gm->program[i].arguments != 0)) {
					if ((int) gm->stack_size < 2) { 
						gm_error(GM_NOT_ENOUGH_ERR);
					}
					if (gm->stack[gm->stack_size-1] % gm->stack[gm->stack_size-2]) {gm_warn(GM_NOT_CLEAN_DIV_WARN);}
					int p = gm->stack[gm->stack_size-1] / gm->stack[gm->stack_size-2];
					gm->stack_size++;
					gm->stack[gm->stack_size-1] = p;
				} else {
					if ((int) gm->stack_size < 1) { 
						gm_error(GM_NOT_ENOUGH_ERR);
					}
					if (gm->stack[gm->stack_size-1] % gm->program[i].arguments) {gm_warn(GM_NOT_CLEAN_DIV_WARN);}
					gm->stack_size++;
					gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] / gm->program[i].arguments;
				}
			} break;
			case GASM_BIT_L: {
				if ((int) gm->stack_size < 1) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				gm->stack_size++;
				gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] << gm->program[i].arguments;
			} break;
			case GASM_BIT_R: {
				if ((int) gm->stack_size < 1) {
					gm_error(GM_NOT_ENOUGH_ERR);
				}
				gm->stack_size++;
				gm->stack[gm->stack_size-1] = gm->stack[gm->stack_size-1] >> gm->program[i].arguments;
			} break;
			case GASM_JMP: {
				if (gm->program_size-1 == gm->program[i].arguments) {
					gm_error(GM_JUMP_ITSELF_ERR);
				}
				// printf("::: %d\n", i);
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
					int r[256] = {0}; int k = 0;
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
					gm_error(GM_INVALID_PRINT_MODE_ERR);
				}
			} break;
			case GASM_IF: {
				// TODO: !CRITICAL! FINISH THE IF STATEMMENT IMPLEMENTATION!! 
				uint64_t     i[256] = {0}; int u = 0;
				char           c[256] = {0};
				while(gm->stack[0] == (uint64_t) 3) {
					i[u] = (int) gm->stack[gm->stack_size];
					gm->stack[gm->stack_size] = '\0';
					--(gm->stack_size); u++;
				}
				memcpy(i, c, u+1);
				for (int p = 0; p < u+1; ++p) printf("%c", i[u-p]);
			}
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