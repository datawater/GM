#pragma once

#define RESET    -1  
#define CLEAR    31

#define FBLACK    0                     
#define FGRAY     8  
#define FGREY     8
#define FRED      1                     
#define FLRED     9
#define FGREEN    2                     
#define FLGREEN   10
#define FYELLOW   3                     
#define FLYELLOW  11
#define FBLUE     4                     
#define FLBLUE    12
#define FMAGENTA  5 
#define FPINK     5     
#define FLMAGENTA 13 
#define FLPINK    13
#define FCYAN     6                     
#define FLCYAN    14
#define FWHITE    7                     
#define FLWHITE   15

#define BBLACK    1+15                        
#define BGRAY     9+15 
#define BRED      2+15                        
#define BLRED     10+15
#define BGREEN    3+15                        
#define BLGREEN   11+15
#define BYELLOW   4+15                        
#define BLYELLOW  12+15
#define BBLUE     5+15                        
#define BLBLUE    13+15
#define BMAGENTA  6+15 
#define BPINK     7+15     
#define BLMAGENTA 14+15 
#define BLPINK    14+15
#define BCYAN     7+15                        
#define BLCYAN    15+15
#define BWHITE    8+15                        
#define BLWHITe   16+15

#define BOLD      32
#define ITALIC    33
#define UNDERLINE 34
#define STRIKE    35

void color(int color);
#define CONSOLE_IMPLEMENTATION

#ifdef CONSOLE_IMPLEMENTATION
#include <stdio.h>

void color(int color) {
    switch (color) {
        case RESET:
            printf("\033[0m");
        break;
        case CLEAR:
            printf("\033[2J");
        break;
        case FBLACK:
            printf("\033[30m");
        break;
        case FRED:
            printf("\033[31m");
        break;
        case FGREEN:
            printf("\033[32m");
        break;
        case FYELLOW:
            printf("\033[33m");
        break;
        case FBLUE:
            printf("\033[34m");
        break;
        case FMAGENTA:
            printf("\033[35m");
        break;
        case FCYAN:
            printf("\033[36m");
        break;
        case FWHITE:
            printf("\033[37m");
        break;
        case FGRAY:
            printf("\033[30m");
        break;
        case FLRED:
            printf("\033[90m");
        break;
        case FLGREEN:
            printf("\033[91m");
        break;
        case FLYELLOW:
            printf("\033[92m");
        break;
        case FLBLUE:
            printf("\033[93m");
        break;
        case FLMAGENTA:
            printf("\033[94m");
        break;
        case FLCYAN:
            printf("\033[96m");
        break;
        case FLWHITE:
            printf("\033[97m");
        break;
        case BBLACK:
            printf("\033[40m");
        break;
        case BRED:
            printf("\033[41m");
        break;
        case BGREEN:
            printf("\033[42m");
        break;
        case BYELLOW:
            printf("\033[43m");
        break;
        case BBLUE:
            printf("\033[44m");
        break;
        case BMAGENTA:
            printf("\033[45m");
        break;
        case BCYAN:
            printf("\033[46m");
        break;
        case BWHITE:
            printf("\033[47m");
        break;
        case BGRAY:
            printf("\033[100m");
        break;
        case BLRED:
            printf("\033[101m");
        break;
        case BLGREEN:
            printf("\033[102m");
        break;
        case BLYELLOW:
            printf("\033[103m");
        break;
        case BLBLUE:
            printf("\033[104m");
        break;
        case BLMAGENTA:
            printf("\033[105m");
        break;
        case BLCYAN:
            printf("\033[106m");
        break;
        case BOLD:
            printf("\033[1m");
        break;
        case ITALIC:
            printf("\033[3m");
        break;
        case UNDERLINE:
            printf("\033[4m");
        break;
        case STRIKE:
            printf("\033[9m");
        break;
    }
}
#endif // CONSOLE_IMPLEMENTATION

// Copyright 2022 datawater <datawater1@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.