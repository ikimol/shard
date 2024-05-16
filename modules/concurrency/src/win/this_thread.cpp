// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include "shard/concurrency/this_thread.hpp"

#include <windows.h>

#pragma pack(push, 8)
struct THREADNAME_INFO {
    DWORD dwType;
    LPCSTR szName;
    DWORD dwThreadID;
    DWORD dwFlags;
};
#pragma pack(pop)

void SetThreadName(const THREADNAME_INFO& info) {
    __try {
        RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    } __except (EXCEPTION_EXECUTE_HANDLER) {}
}

namespace shard::concurrency::this_thread {

void set_name(const char* name) {
#if defined _MSC_VER
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name;
    info.dwThreadID = GetCurrentThreadId();
    info.dwFlags = 0;
    SetThreadName(info);
#endif
}

} // namespace shard::concurrency::this_thread
