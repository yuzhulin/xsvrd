// License: GPL
//
// Author: xus
// E-Mail: xushvai@gmail.com
// 
// Overview:
// Implementation for DynamicLibLoader class.

#include "dynamiclibloader.h"

DynamicLibLoader::DynamicLibLoader()
	: filename_(NULL), library_handle_(NULL)
{
}

DynamicLibLoader::~DynamicLibLoader()
{

}

void DynamicLibLoader::Init(const char* filename)
{
	filename_ = filename;
}

Func DynamicLibLoader::GetExportFuncPtr()
{

	return NULL;
}

void* DynamicLibLoader::GetExportFuncAddress(const char* func_name)
{
#if (defined(_WIN32))

#elif (defined(__gnu_linux__))
#endif

	return NULL;
}

HMODULE DynamicLibLoader::Load()
{
#if (defined(_WIN32))
	return ::LoadLibrary(filename_);
#elif (defined(__gnu_linux__))
#endif
}

void DynamicLibLoader::Free()
{

}