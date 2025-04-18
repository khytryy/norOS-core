#pragma once

#define COM1 0x3f8
#define ICON_SUCCESS "[ SUCCESS ]"
#define ICON_FAILURE "[ FAILED ]"

#include "../../io.h"
#include <stdint.h>

int serialBegin();

int is_transmit_empty();

void serialWriteChar(char l);

void serialWrite(const char* string);

void itoa(unsigned long value, char* str, int base);