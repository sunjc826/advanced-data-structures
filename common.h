#ifndef ads_common_h
#define ads_common_h

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CONCAT2_(a, b) a##b
#define CONCAT2(a, b) CONCAT2_(a, b)
#define CONCAT3_(a, b, c) a##b##c
#define CONCAT3(a, b, c) CONCAT3_(a, b, c)
#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)
#define SWAP(type, a, b) \
    do                   \
    {                    \
        type tmp = a;    \
        a = b;           \
        b = tmp;         \
    } while (false);
#endif