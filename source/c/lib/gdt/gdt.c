/*
    gdt 41925 by Yehor Khytryy
    created for norOS-core project
*/

#include "gdt.h"

typedef struct {
    uint16_t limitLow;               // limit (bits 0-15)
    uint16_t baseLow;                // base (bits 0-15)
    uint8_t baseMiddle;              // base (bits 16-23)
    uint8_t access;                  // access
    uint8_t flagsLimitHigh;          // limit (bits 16-19)
    uint8_t baseHigh;                // base (bits 24-31)
} __attribute__((packed)) entryGDT;

typedef struct {
    uint16_t limit;                  // sizeof(gdt) -1
    entryGDT* address;               // address

} __attribute__((packed)) descriptorGDT;

typedef enum {
    gdtAccessCodeReadable            = 0x02,
    gdtAccessDataWriteable           = 0x02,

    gdtAccessCodeConfirming          = 0x04,
    gdtAccessDataDirectionNormal     = 0x00,
    gdtAccessDataDirectionDown       = 0x04,

    gdtAccessDataSegment             = 0x10,
    gdtAccessCodeSegment             = 0x18,

    gdtAccessDescriptorTSS           = 0x00,

    gdtAccessRing0                   = 0x00,
    gdtAccessRing1                   = 0x20,
    gdtAccessRing2                   = 0x40,
    gdtAccessRing3                   = 0x60,

    gdtAccessPresent                 = 0x80,
} accessGDT;

typedef enum {
    gdtFlag64                        = 0x20,
    gdtFlag32                        = 0x40,
    gdtFlag16                        = 0x00,

    gdtFlagGranularity1B             = 0x00,
    gdtFlagGranularity4K             = 0x80,
} flagsGDT;

#define gdtLimitLow(limit)                  (limit & 0xFFFF)
#define gdtBaseLow(base)                    (base & 0xFFFF)
#define gdtBaseMiddle(base)                 ((base >> 16) & 0xFF)
#define gdtFlagsLimitHigh(limit, flags)     (((limit >> 16) & 0xF) | (flags & 0xF0))
#define gdtBaseHigh(base)                   ((base >> 24) & 0xFF)


#define entryGDT(base, limit, access, flags) { \
    gdtLimitLow(limit),                     \
    gdtBaseLow(base),                       \
    gdtBaseMiddle(base),                    \
    access,                                 \
    gdtFlagsLimitHigh(limit, flags),        \
    gdtBaseHigh(base),                      \
}

entryGDT gGDT[] = {

    // null descriptor
    entryGDT(0, 0, 0, 0),

    // kernel 64-bit code segment
    entryGDT(0, 0xFFFFF, 
        gdtAccessPresent | gdtAccessRing0 | gdtAccessCodeSegment | gdtAccessCodeReadable, 
        gdtFlag64 | gdtFlagGranularity4K),

    entryGDT(0, 0xFFFFF, 
        gdtAccessPresent | gdtAccessRing0 | gdtAccessDataSegment | gdtAccessDataWriteable, 
        gdtFlag64 | gdtFlagGranularity4K),

};

descriptorGDT gDescriptorGDT = { sizeof(gGDT) - 1, gGDT};

void i686GDTload(descriptorGDT* descriptor, uint16_t codeSegment, uint16_t dataSegment);