#pragma once
#include <stdbool.h>

void todo(char* msg, int line, char* file, int failornot);

void error (char* msg);
void warn (char* msg);
void info   (char* msg);

bool file_exist(char* path);
char* strdup(const char* s);
char* strchp(char* s, size_t n);
char* trim(char * s);

#ifdef UTILS_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/stat.h>

#define CONSOLE_IMPLEMENTATION
#include "console.h"

void todo(char* msg, int line, char* file, int failornot) {
    color(FRED);
    printf("[TODO] ");
    color(RESET);
    printf("%s | File: %s, Line:%d\n", msg, file, line);
    !failornot ? exit(1) : (void) 0;
}

void error(char* msg) {
    color(FRED);
    printf("[ERROR] ");
    color(RESET);
    printf("%s\n" ,msg);
    exit(1);
}

void warn(char* msg) {
    color(FYELLOW);
    printf("[WARN] ");
    color(RESET);
    printf("%s\n" ,msg);
}

void info(char* msg) {
    color(FLBLUE);
    printf("[INFO] ");
    color(RESET);
    printf("%s\n" ,msg);
}

bool file_exist(char* path) {
    struct stat  buffer;   
    return (stat (path, &buffer) == 0);
}

char* strdup(const char* s) {
    char* st = malloc(strlen(s)+1);
    strcpy(st, s);
    return st;
}

char* strchp(char* s, size_t n) {
    if (n > (strlen(s)+1)) { return NULL;}
    char* tr = malloc(n+1); strncpy(tr, s, n); tr[n] = '\0';
    return tr;
}

char* trim(char * s) {
    int l = strlen(s);

    while(isspace(s[l - 1])) --l;
    while(* s && isspace(* s)) ++s, --l;

    return s;
}

#endif // UTILS_IMPLEMENTATION