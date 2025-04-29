#pragma once

#define COM1 0x3f8

#include "../../io.h"
#include <stdint.h>

int serialBegin();

int is_transmit_empty();

void serialWriteChar(char l);

void serialWrite(char* string);

void itoa_hex(unsigned int value, char* buffer);

void newline();

void itoa_dec(unsigned int value, char* buffer);