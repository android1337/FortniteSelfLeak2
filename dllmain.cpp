#include "fort.hpp"

bool DllMain(uintptr_t hModule, uintptr_t ul_reason_for_call, uintptr_t lpReserved)
{
    if (ul_reason_for_call != 1) return false;

    fort::init();

    return true;
}