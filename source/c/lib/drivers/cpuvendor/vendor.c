#include "vendor.h"

const char* getcpumanu() {
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

char* getcpumodel() {
    static char cpu_name[49]; // 48 chars + null terminator
    uint32_t eax, ebx, ecx, edx;
    uint32_t* data = (uint32_t*) cpu_name;

    // supports cpu model?
    if (__get_cpuid_max(0x80000000, NULL) < 0x80000004) {
        cpu_name[0] = '\0';
        return cpu_name;
    }

    for (uint32_t i = 0; i < 3; i++) {
        __get_cpuid(0x80000002 + i, &eax, &ebx, &ecx, &edx);
        data[i * 4 + 0] = eax;
        data[i * 4 + 1] = ebx;
        data[i * 4 + 2] = ecx;
        data[i * 4 + 3] = edx;
    }

    cpu_name[48] = '\0'; // null terminator
    return cpu_name;
}