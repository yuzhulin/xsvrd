// Implementation for DynamicLibLoader class.
//
// Author: xus
// E-Mail: xushvai@gmail.com
// License: GPL

#include "dynamiclibloader.h"

DynamicLibLoader::DynamicLibLoader()
	: filename_(NULL), library_handle_(NULL), export_function_ptr_(NULL)
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
	if (!export_function_ptr_) {
		if (Load()) {
			export_function_ptr_ = (Func)(GetExportFuncAddress("CreateObj"));
		} else {
			Free();
		}
	}
	return export_function_ptr_;
}

void* DynamicLibLoader::GetExportFuncAddress(const char* func_name)
{
#if (defined(_WIN32))
	return GetProcAddress(library_handle_, func_name);
#elif (defined(__gnu_linux__))
#endif
}

HMODULE DynamicLibLoader::Load()
{
#if (defined(_WIN32))
	library_handle_ = ::LoadLibrary(filename_);
#elif (defined(__gnu_linux__))
#endif
	return library_handle_;
}

void DynamicLibLoader::Free()
{
#if (defined(_WIN32))
	::FreeLibrary(library_handle_);
#elif
#endif
}