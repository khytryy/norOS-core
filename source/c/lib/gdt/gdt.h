#pragma once
#include <stdint.h>

#include <stdint.h>

typedef struct {
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t baseMiddle;
    uint8_t access;
    uint8_t flagsLimitHigh;
    uint8_t baseHigh;
} __attribute__((packed)) entryGDT;

typedef struct {
    uint16_t limit;
    entryGDT* address;
} __attribute__((packed)) descriptorGDT;

extern descriptorGDT gDescriptorGDT;

void i686GDTload(descriptorGDT* descriptor, uint16_t codeSegment, uint16_t dataSegment);