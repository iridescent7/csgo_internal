#pragma once
#include <Windows.h>

#define NETVAR_GETSET(type, name)                 \
    type& name##() {                              \
        return *(type*)((DWORD)this + O::##name); \
	}

#define PNETVAR_GET(type, name)               \
    type* name##() {                             \
        return (type*)((DWORD)this + O::##name); \
	}

inline void**& getvtable(void* base) {
	return *(void***)(base);
}

template <typename Fn>
inline Fn getvfunc(void* base, size_t index) {
	return (Fn)(getvtable(base)[index]);
}
