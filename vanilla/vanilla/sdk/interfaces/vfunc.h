#pragma once

template <typename Fn>
__forceinline Fn GetVFunc(void* ppClass, int index)
{
	using ptr_type = void*;
	ptr_type* vftable = *(ptr_type**)ppClass;

	return (Fn)(vftable[index]);
}
