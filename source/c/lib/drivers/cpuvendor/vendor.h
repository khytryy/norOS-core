#pragma once

#include <cpuid.h>
#include <stdint.h>

static const char* getmodel(void) {
    static char vendor[13];
    uint32_t eax, ebx, ecx, edx;

    // get vendor string
    __cpuid(0, eax, ebx, ecx, edx);

    // vendor string comes in EBX, EDX, ECX
    ((uint32_t*)vendor)[0] = ebx;  // first 4 chars
    ((uint32_t*)vendor)[1] = edx;  // next 4 chars
    ((uint32_t*)vendor)[2] = ecx;  // last 4 chars
    vendor[12] = '\0';             // null terminate

    return vendor;
}
