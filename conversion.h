#ifndef CONVERSION_H
#define CONVERSION_H

#include <stdio.h>
#include <stdlib.h>

/* single, portable CLEAR_CMD definition */
#ifndef CLEAR_CMD
#ifdef _WIN32
#define CLEAR_CMD "cls"
#else
#define CLEAR_CMD "clear"
#endif
#endif

/* public API from conversion.c */
void calculateGPA(void);
void calculateCGPA(void);
void method(void);
void flush_input(void);
void sgpa(void);

#endif /* CONVERSION_H */
