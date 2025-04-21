#include "gdt.h"

typedef enum {
    gdtAccessCodeReadable            = 0x02,
    gdtAccessDataWriteable           = 0x02,

    gdtAccessCodeConfirming          = 0x04,
    gdtAccessDataDirectionNormal     = 0x00,
    gdtAccessDataDirectionDown       = 0x04,

    gdtAccessDataSegment             = 0x10,
    gdtAccessCodeSegment             = 0x18,

    gdtAccessDescriptorTSS           = 0x09,

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
#define gdtFlagsLimitHigh(limit, flags)     (((limit >> 16) & 0x0F) | (flags & 0xF0))
#define gdtBaseHigh(base)                   ((base >> 24) & 0xFF)

#define entryGDT(base, limit, access, flags) { \
    gdtLimitLow(limit),                     \
    gdtBaseLow(base),                       \
    gdtBaseMiddle(base),                    \
    access,                                 \
    gdtFlagsLimitHigh(limit, flags),        \
    gdtBaseHigh(base),                      \
}

// GDT Table
entryGDT gGDT[] = {
    entryGDT(0, 0, 0, 0),  // Null descriptor

    // Kernel 64-bit code segment (index = 1)
    entryGDT(0, 0x000FFFFF, 
        gdtAccessPresent | gdtAccessRing0 | gdtAccessCodeSegment | gdtAccessCodeReadable, 
        gdtFlag64 | gdtFlagGranularity4K),

    // Kernel 64-bit data segment (index = 2)
    entryGDT(0, 0x000FFFFF, 
        gdtAccessPresent | gdtAccessRing0 | gdtAccessDataSegment | gdtAccessDataWriteable, 
        gdtFlag64 | gdtFlagGranularity4K),
};

// GDT Descriptor
descriptorGDT gDescriptorGDT = {
    .limit = sizeof(gGDT) - 1,
    .address = (uint64_t)&gGDT
};
