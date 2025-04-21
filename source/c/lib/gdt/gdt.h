// gdt.h for norOS-core 42125

#pragma once
#include <stdint.h>

typedef struct {
    uint16_t limitLow;           // Limit bits 0-15
    uint16_t baseLow;            // Base bits 0-15
    uint8_t baseMiddle;          // Base bits 16-23
    uint8_t access;              // Access byte
    uint8_t flagsLimitHigh;      // Flags + Limit bits 16-19
    uint8_t baseHigh;            // Base bits 24-31
} __attribute__((packed)) entryGDT;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) descriptorTSS;  // Optional

typedef struct {
    uint16_t limit;
    uint64_t address;
} __attribute__((packed)) descriptorGDT;

extern descriptorGDT gDescriptorGDT;